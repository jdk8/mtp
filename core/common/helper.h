#ifndef UTIL_H__
#define UTIL_H__

#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <sys/timeb.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/crc.hpp>
#include "logger.h"

namespace mtp {

class TimeHelper {
 public:
  static uint64_t CurrentTimeMillis() {
    struct timeb tp;
    ftime(&tp);
    return tp.time * 1000 + tp.millitm;
  }
};

class CrcHelper {
 public:
 static int Crc16(const std::string& s) {
    boost::crc_16_type result;
    result.process_bytes(s.data(), s.length());
    return result.checksum();
  }
};

class FileHelper {
 public:
  static std::vector<std::string> GetFileList(std::string dir) {
    DIR *p_dir;
    struct dirent *p_dirent;
    std::vector<std::string> files;

    if((p_dir = opendir(dir.c_str())) == NULL) {
        LOG_ERROR << "Open dir:" << dir << "failed";
        return files;
    }
    while((p_dirent = readdir(p_dir))) {
      std::string f(p_dirent->d_name);
      if(f != "." && f != "..") {
        files.push_back(f);
      }
    }
    closedir(p_dir);
    return files;
  }
};

class StringHelper {
 public:
  static std::vector<std::string> Split(std::string s, std::string delimiter) {
    std::vector<std::string> vec;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
      token = s.substr(0, pos);
      vec.push_back(token);
      s.erase(0, pos + delimiter.length());
    }
    vec.push_back(s);
    return vec;
  }

  static bool IsNumeric(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) {
      ++it;
    }
    return !s.empty() && it == s.end();
  }
};

class SocketHelper {
 public:
  static int OpenListenfd(int port) {
    int listen_fd, opt_val = 1;
    struct sockaddr_in server_addr;
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      LOG_ERROR << "Server socket error";
      return -1;
    }
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR,
      (const void*)&opt_val, sizeof(opt_val)) < 0) {
      LOG_ERROR << "Server setsockopt SO_REUSEADDR error";
    }
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)port);
    if (bind(listen_fd, (const sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
      LOG_ERROR << "Server bind error";
      return -1;
    }

    if (listen(listen_fd, SOMAXCONN) < 0) {
      LOG_ERROR << "Server listen error";
      return -1;
    }
    return listen_fd;
  }

  static int OpenClienfd(char* hostname, int port) {
    int client_fd;
    struct hostent* hp;
    struct sockaddr_in server_addr;
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      LOG_ERROR << "Client socket error";
      return -1;
    }
    if ((hp = gethostbyname(hostname)) == NULL) {
      LOG_ERROR << "Client gethostbyname error";
      return -2;
    }
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char*)hp->h_addr,
      (char*)&server_addr.sin_addr.s_addr, hp->h_length);
    server_addr.sin_port = htons(port);

    if (connect(client_fd, (const sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
      LOG_ERROR << "Client connect error";
      return -1;
    }
    return client_fd;
  }

  static int SetSocketNonBlock(int conn_fd) {
    int flags, r;
    if ((flags = fcntl(conn_fd, F_GETFL, 0)) == -1) {
      LOG_ERROR << "SetSocketNonBlock F_GETFL error";
      return -1;
    }

    flags |= O_NONBLOCK;
    if ((r = fcntl(conn_fd, F_SETFL, flags)) == -1) {
      LOG_ERROR << "SetSocketNonBlock F_SETFL error";
      return -1;
    }
    return 1;
  }

  static int Accept(int listen_fd) {
    struct sockaddr_in client_addr;
    struct hostent* hp;
    int conn_fd, client_len;
    client_len = sizeof(client_addr);
    if ((conn_fd = accept(listen_fd, (sockaddr*)&client_addr, (socklen_t*)&client_len)) < 0) {
      LOG_ERROR << "Accept error";
      return -1;
    }
    hp = gethostbyaddr((const char*)&client_addr.sin_addr.s_addr,
      sizeof(client_addr.sin_addr.s_addr), AF_INET);
    char* haddrp = inet_ntoa(client_addr.sin_addr);
    LOG_INFO << "Server connected to " << hp->h_name << " " << haddrp;
    return conn_fd;
  }
};

class UUIDHelper {
 public:
  static std::string GenId() {
    return boost::uuids::to_string(boost::uuids::random_generator()());
  }
};

}

#endif