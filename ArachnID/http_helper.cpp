#include "http_helper.h"
#include <bits/stdc++.h>

using namespace std;

//class HTTP_parser {
//    private:
//        QString host;
//        QString content;
//        QString size;

//    HTTP_parser(string) {

//    }
//};


string getHostFromHTTPRequest(string request) {
    string target = "\r\nHost: ";
    for(int i = 0; i < int(request.size()); i++) {
        for(int j = i; j < int(target.size())+ i and j < int(request.size()); j++) {
            if(target[j-i] != request[j]) break;
            if(j == int(target.size()) + i - 1) {
                string result = "";
                for(int k = j + 1; k < int(request.size()) - 1; k++) {
                    if(request[k] == '\r' and request[k+1] == '\n') {
                        break;
                    }
                    result += request[k];
                }
                return result;
            }
        }
    }
//    throw exception("kekek");
}
