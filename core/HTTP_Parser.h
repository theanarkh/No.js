#ifndef HTTP_Parser_H
#define HTTP_Parser_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <ctime>
#include "../include/llhttp/llhttp.h"

#define MAX_LEN 2048

using namespace std;

namespace No
{
    namespace HTTP {
        class HTTP_Parser {
            public:
                HTTP_Parser(llhttp_type type) {
                    llhttp_init(&parser, type, &HTTP_Parser::settings);
                    // set data after llhttp_init, because llhttp_init will call memset to fill zero to memory 
                    parser.data = this;
                }
            
                int on_message_begin(llhttp_t* parser);
                int on_status(llhttp_t* parser, const char* at, size_t length);
                int on_url(llhttp_t* parser, const char* at, size_t length);
                int on_header_field(llhttp_t* parser, const char* at, size_t length);
                int on_header_value(llhttp_t* parser, const char* at, size_t length);
                int on_headers_complete(llhttp_t* parser);
                int on_body(llhttp_t* parser, const char* at, size_t length);
                int on_message_complete(llhttp_t* parser);
                int parse(const char* data, int len);
                void print();
            private: 
                unsigned char major_version;
                unsigned char minor_version;
                unsigned char upgrade;
                unsigned char keepalive;
                time_t parse_start_time;
                time_t header_end_time;
                time_t message_end_time;
                string url;
                string status;
                vector<string> keys;
                vector<string> values;
                string body;
                llhttp_t parser;
                static llhttp_settings_t settings;
        };
    }
}

#endif