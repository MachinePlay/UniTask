#ifndef _EchoServantImp_H_
#define _EchoServantImp_H_

#include "servant/Application.h"
#include "proto/EchoServant.h"

/**
 *
 *
 */
class EchoServantImp : public test::EchoServant
{
public:
    /**
     *
     */
    virtual ~EchoServantImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    /**
     *
     */
    virtual int test(tars::TarsCurrentPtr current) { return 0;};
};
/////////////////////////////////////////////////////
#endif
