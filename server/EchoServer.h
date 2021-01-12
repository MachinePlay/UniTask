#ifndef _EchoServer_H_
#define _EchoServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class EchoServer : public Application
{
public:
    /**
     *
     **/
    virtual ~EchoServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern EchoServer g_app;

////////////////////////////////////////////
#endif
