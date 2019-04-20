//============================================================================
// Name        : cobContext.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __COB_PROBE_H__
#define __COB_PROBE_H__

#include "llfsm/lianli.h"
#include "cobMacros.h"

NS_COB_BEGIN

class Bin;
class Probe
{
    friend class Bin;
public:
	Probe(const std::string& name);

    Bin* getBin() {return mBin;}
    const std::string& getName() const {return mName;}

    bool bind(const std::string& config);
    bool unbind();

    virtual bool request(const std::string& evtName, const lianli::EvtData& evtData, lianli::EvtData& retData);
	virtual bool response(const lianli::EvtData& resultData, bool bRightNow = false);

protected:
    virtual void onRequest(const std::string& evtName, const lianli::EvtData& evtData, lianli::EvtData& retData);
    virtual void onResponse(const std::string& evtName, lianli::EvtData& retData);
    virtual void onNotify(const std::string& evtName, const lianli::EvtData& evtData);

protected:
    Bin* mBin;
    std::string mName;
};

NS_COB_END

#endif
