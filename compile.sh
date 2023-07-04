#!/bin/bash
time g++ -o main http.cpp -pthread -lcrypto -lssl -Wall ; ./main