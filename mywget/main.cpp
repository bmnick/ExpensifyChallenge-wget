//
//  main.cpp
//  mywget
//
//  Created by Ben Nicholas on 11/17/11.
//  Copyright (c) 2011 Oracle Corporation. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>

void usage(void);
std::string get_url(const char* url);
bool write_file(std::string outfile_name, std::string contents);

int main (int argc, const char * argv[])
{
    if (argc < 2) {
        usage();
        return 1;
    }

    std::string site_contents = get_url(argv[1]);
    
    write_file("wget.out", site_contents);
    
    return 0;
}

void usage(void) {
    std::cout << "Usage: mywget url" << std::endl;
}

std::string get_url(const char* url) {
    return "Hello there\n";
}

bool write_file(std::string outfile_name, std::string contents) {
    std::ofstream out_file(outfile_name.c_str());
    
    if (out_file.is_open()) {
        out_file << contents;
    } else {
        return false;
    }
    
    out_file.close();
    
    return true;
}
