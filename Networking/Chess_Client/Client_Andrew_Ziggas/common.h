#ifdef WIN32
#include <windows.h>
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif

#include <stdexcept>
#include <string>
#include <vector>
#include <string.h>
#include <iostream>
#include <sstream>

inline 
void sendall(int sd, const void* bufv, int size)
{
    char* buf = (char*) bufv;
    while(size > 0){
        //last item is flags
        int rv = send(sd,buf,size,0);
        if( rv <= 0 )
            throw std::runtime_error("socket error");
        buf += rv;
        size -= rv;
    }
}

inline
std::string readline(int s){
    std::string rets;
    std::vector<char> buffer;
    buffer.resize(1024);
    while(1){
        //read a block of characters into the buffer
        int rv = recv(s,&buffer[0],buffer.size()-1,MSG_PEEK);
        if( rv <= 0 )
            break;
            
        //scan for a newline
        int i;
        for(i=0;i<rv;++i){
            if( buffer[i] == '\n' ){
                break;
            }
        }
        
        //if we found a newline, insert all characters up to the newline.
        //if we didn't find a newline, insert all characters in the buffer
        rets.insert( rets.end(), buffer.begin(), buffer.begin()+i);
        
        //remove all characters up to the newline (if there was one) from
        //the OS's buffer
        recv(s,&buffer[0],i+1,0);
        
        //return if we found a newline
        if( i != rv )
            break;
    }
    
    return rets;
}

std::string trim(const std::string& s){
    unsigned start=0;
    while(start < s.length() && isspace(s[start]))
        start++;
    if( start == s.length() )
        return std::string("");
    unsigned end = s.length()-1;
    while(isspace(s[end]))
        end--;
    return s.substr(start,end-start+1);
}

//split the string. If delim is "", split on any whitespace. maxsplit
//works like Python.
std::vector<std::string> split(const std::string& s, std::string delim, int maxsplit){
        
    std::vector<std::string> tmp;
    std::istringstream iss(s);
    for(int i=0; i < maxsplit; i++){
        std::string word;
        if( maxsplit != -1 && i == maxsplit )
            getline(iss,word,'\0');
        else if( delim.empty())
            iss >> word;
        else
            getline(iss,word,delim[0]);
        if( iss.fail() )
            break;
        tmp.push_back(word);
    }
    return tmp;
}

std::vector<std::string> split(const std::string& s){
    return split(s,"",-1);
}

        
    
    
