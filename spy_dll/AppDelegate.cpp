#include "AppDelegate.h"
#include "probes/MemSpy.h"

AppDelegate::AppDelegate()
{

}

void AppDelegate::onCreate(const lianli::Context& context)
{
    mBin.create(m_hMainWnd);

    //auto prober1 = new Prober1("prober1");
    //mBin.install(*prober1);

    auto memSpy = new MemSpy("memspy");
    mBin.install(*memSpy);
}

void AppDelegate::onStart()
{
    mBin.pipeListen(0x1234);

}

void AppDelegate::onStop()
{

}

void AppDelegate::onDestroy(const lianli::Context& context)
{
    mBin.destroy();
}
