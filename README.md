# WebScraper
Simple web scraper in c++ using curl and libxml2 libraries.

Compile

- Linux 
  g++ main.cpp scraper.cpp -pthread -std=c++11 -o webScraper $(pkg-config --cflags --libs libxml-2.0 libcurl)
  
- Windows
  I need to find a Windows Machine
