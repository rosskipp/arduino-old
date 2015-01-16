#include "Arduino.h"
#include <SPI.h>
#include <Ethernet.h>
#include "plotly_ethernet.h"

#include <avr/dtostrf.h>

plotly::plotly(){
    width_ = 10;
    prec_ = 5;
    VERBOSE = true;
    DRY_RUN = true;
    maxStringLength = 0;
    layout = "{}";
    world_readable = true;
    timestamp = false;
    timezone = "America/Montreal";
}

void plotly::open_stream(int N, int M, char *filename_, char *layout){
    N_ = N;  // total sets of data sent = number of rows in plotly-json data matrix
    M_ = (M*2); // total number of traces * 2 = number of columns in plotly-json data matrix
    ni_ = 0; // counter of number of sets of data set (number of rows in the plotly-json data matrix) transmitted
    mi_ = 0; // counter of points sent in each row of data
    nChar_ = 0; // counter of number of characters transmitted
    filename=filename_;
    delay(1000);
    if(DRY_RUN){ Serial.println("This is a dry run, we are not connecting to plotly's servers..."); }
    else{
      if(VERBOSE) { Serial.println("Attempting to connect to plotly's servers..."); }

      char server[] = "plot.ly";
      while ( !client.connect(server, 80) ) {
        if(VERBOSE){ Serial.println("Couldn\'t connect to servers.... trying again!"); }
        delay(1000);
      }
    }
    if(VERBOSE) Serial.println("Connected to plotly's servers");
    if(VERBOSE) Serial.println("\n== Sending HTTP Post to plotly ==");

    // HTTP Meta
    println_("POST /clientresp HTTP/1.1", 0);
    println_("Host: 107.21.214.199", 0);
    println_("User-Agent: Arduino/2.0", 0);

    // compute an upper bound on the post body size
    upper_ = 273+strlen(layout)+((N_*M_-1)*2)+((N_-1)*4)+(max(20,maxStringLength)*N_*M_);
      /* Computation composition: 
        44 // First part of querystring below
        + 30 // Upper limit on username length
        + 5  // "&key="
        + 10 // api key length
        + 6  // "&args="
        + 22 // "&kwargs={\"filename\": \""
        + 100 // upper bound on filename
        + 53 // "\", \"fileopt\": \"extend\", \"transpose\": true, \"layout\": "
        + layout.length()
        + 1 // closing }
        + (N_*M_-1)*2   // + 2-chars for comma and space for all but the last numbers ... 
        + (N_-1)*4  // + 4-chars for square brackets, comma, space for n-1 set of points [], 
        + 2         // + 2-chars for start 'n finish square braces
        + max(20,maxStringLength)*N_*M_;  // + max character buffer in converting floats to strings. NOTE: The largest float is 56 chars... should the buffer be this big? [(48-chars for largest float integer left of the decimal (-3.4028235E+38) )+(1 decimal pt)+(6 digits of precision right of the decimal)=56-chars]*each float
     */   

    if(timestamp){
      upper_ += 122;
      /* Computation composition: 
      upper_ += 26 // \"convertTimestamp\": true"
        + 41 // ", \"convertTimestamp\": true, \"timezone\": \""
        + 30 // upper bound on timezones string
        + 15 // "\", \"sentTime\": "
        + 10; // max length of unsigned long for sentTime: 4,294,967,295
      */
    }

    // send the header to plotly
    print_("Content-Length: ", 0);
    println_(upper_, 0);
    println_("", 0);

    // start the post string
    print_("version=0.2&origin=plot&platform=arduino&un=", 44);
    print_(username);
    print_("&key=", 5);
    print_(api_key);
    print_("&args=", 6);
}

void plotly::close_stream(){
    print_( "&kwargs={\"filename\": \"", 22 );
    print_( filename );
    print_( "\", \"fileopt\": \"extend\", \"transpose\": true, \"layout\": ", 53);    

    print_( layout );

    if(!world_readable){
        print_( ", \"world_readable\": false", 25);
    }

    if(timestamp){
      print_( ", \"convertTimestamp\": true", 26 );
      print_( ", \"timezone\": \"", 15 );
      print_(timezone);
      print_("\"", 1);
      print_( ", \"sentTime\": ", 14 );
      print_( millis() );
      print_( "}", 1 );
    } else{
      print_( "}", 1);
    }

    // fill the remainder of the post with white space
    if(nChar_>=upper_){ 
      Serial.print("Error: Content-Length upper bound is too small. Upper bound was: ");
      Serial.print(upper_); Serial.print(", and we printed "); Serial.print(nChar_); Serial.println(" characters.");
      Serial.println("Message will not transmit, report bug to chris at chris@plot.ly.");
    }
    for(int i=nChar_; i<upper_; i++){
        if(!DRY_RUN) client.print(" ");
    }
    // final newline to terminate the post
    println_("", 0);
    if(VERBOSE) Serial.println("== Sent message, waiting for plotly's response ==");
    if(!DRY_RUN && VERBOSE){
        while(client.connected()){
            if(client.available()){
                char c = client.read();
                Serial.print(c);            
            }
        }
        client.stop();
    }
    return;
}

// overloaded functions that print pairs of data to the client
void plotly::post(int x, int y){     sendString_(x); sendString_(y); }
void plotly::post(int x, float y){   sendString_(x); sendString_(y); }
void plotly::post(unsigned long x, int y){     sendString_(x); sendString_(y); }
void plotly::post(unsigned long x, float y){   sendString_(x); sendString_(y); }
void plotly::post(float x, int y){   sendString_(x); sendString_(y); }
void plotly::post(float x, float y){ sendString_(x); sendString_(y); }
void plotly::post(char *x, int y){   sendString_(x); sendString_(y); }
void plotly::post(char *x, float y){ sendString_(x); sendString_(y); }
void plotly::post(String x, int y){  sendString_(x); sendString_(y); }
void plotly::post(String x, float y){sendString_(x); sendString_(y); }

// overloaded functions that print data to the client as per JSON format
void plotly::sendString_(String d){
  send_prepad_();
  print_("\"",1); print_(d); print_("\"",1);
  send_postpad_();
}
void plotly::sendString_(char *d){
  send_prepad_();
  print_("\"",1); print_(d); print_("\"",1);
  send_postpad_();
}
void plotly::sendString_(float d){
    send_prepad_();
    char s_[width_];
    dtostrf(d,width_,prec_,s_);
    print_(s_,width_);
    send_postpad_();
}
void plotly::sendString_(int d){
  send_prepad_();
  print_(d); 
  send_postpad_();
}
void plotly::sendString_(unsigned long d){
  send_prepad_();
  print_(d); 
  send_postpad_();
}
void plotly::print_(char *s, int nChar){
  if(VERBOSE){ Serial.print(s); }
  if(!DRY_RUN) { client.print(s); }
  nChar_ += nChar;
}
void plotly::print_(char *s){
  if(VERBOSE){ Serial.print(s); }
  if(!DRY_RUN) { client.print(s); }
  nChar_ += strlen(s);
}
void plotly::print_(String s, int nChar){
  if(VERBOSE){ Serial.print(s); }
  if(!DRY_RUN) { client.print(s); }
  nChar_ += nChar;
}
void plotly::print_(String s){
  if(VERBOSE){ Serial.print(s); }
  if(!DRY_RUN) { client.print(s); }
  nChar_ += s.length();
}
void plotly::print_(int s){
  if(VERBOSE){ Serial.print(s); }
  if(!DRY_RUN) { client.print(s); }
  nChar_ += len_(s);  
}
void plotly::print_(unsigned long s){
  if(VERBOSE){ Serial.print(s); }
  if(!DRY_RUN) { client.print(s); }
  nChar_ += len_(s);
}
void plotly::println_(char *s, int nChar){
  if(VERBOSE){ Serial.println(s); }
  if(!DRY_RUN) { client.println(s); }
  nChar_ += nChar;
}
void plotly::println_(unsigned long int s, int nChar){
  if(VERBOSE){ Serial.println(s); }
  if(!DRY_RUN) { client.println(s); }
  nChar_ += nChar;
}
int plotly::len_(int i){
  if(i > 9999) return 5;
  else if(i > 999) return 4;
  else if(i > 99) return 3;
  else if(i > 9) return 2;
  else return 1;
}
int plotly::len_(unsigned long i){
  if(i > 999999999) return 10;
  else if(i > 99999999) return 9;
  else if(i > 9999999) return 8;
  else if(i > 999999) return 7;
  else if(i > 99999) return 6;
  else if(i > 9999) return 5;
  else if(i > 999) return 4;
  else if(i > 99) return 3;
  else if(i > 9) return 2;
  else return 1;
}
void plotly::send_prepad_(){
  // print [[ or [ to the client if the start of the matrix or row 
  mi_ += 1;
  if(mi_ == 1){
      ni_ += 1;
      if(ni_==1){
          print_("[[",2);
      } else{
          print_("[",1);
      }
  }
}
void plotly::send_postpad_(){
  // print ", ", ", ]", "], ", "]]" depending on the value of the mi_, ni_ counters
  if(mi_ == 1){
      if(ni_==1){
          print_(", ", 2);
      } else{
          print_(", ", 2);
      }
  } else if(mi_ == M_ && ni_ < N_){
      print_("], ", 3);
      mi_ = 0;
  } else if(mi_ == M_ && ni_ == N_){
      print_("]]", 2);
  } else{
      print_(", ", 2);
  }
  if(mi_ == M_ && ni_ == N_){ 
      close_stream(); 
  }
}
