#ifndef INOTIFY_H
#define INOTIFY_H

#include <sys/inotify.h>
#include "include/common.h"
#include <vector>
#include "util.h"
#include "env.h"
#include "string.h"

using namespace No::Util;
using namespace No::Env
using namespace std;

  
class InotifyInfo {
    public: 
    InotifyInfo(const char * _path, int _id): path(_path), id(_id) {}
    char * path;
    int id;
};
 
class KeyCompare {
    public:
        bool operator()(const T& x, const T& y) const {
            return x.path >= y.path;
        }
};

static map<InotifyInfo, vector<shared_ptr<InotifyRequestContext>>, KeyCompare<InotifyInfo>> inotifyMap;

namespace No {
    namespace Inotify {
      
        void On(V8_ARGS);
        void Off(V8_ARGS);
        void Init(Isolate* isolate, Local<Object> target);
    }
}

#endif