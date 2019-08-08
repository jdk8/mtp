#ifndef CONST_H__
#define CONST_H__

namespace mtp {

static const int kMaxRpcSize = 1024;
static const int kMaxEpollEvent = 1024;
static const int kEpollWaitTimeout = 1;
static const int kRingBufferSize = 1024;
static const int kWorkerNum = 4;
static const int KWorkerTickInterval = 1;
static const int kMaxMessageProceedPerTick = 100;
static const std::string kServerId = "Server";
static const int kMaxFreeObjectCollect = 1000;
static const int kMaxFreeObjectReturn = 1000;
static const bool kDebug = false;

}

#endif