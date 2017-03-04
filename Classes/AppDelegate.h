#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

/**
@brief    The cocos2d Application.

<<<<<<< HEAD
Private inheritance here hides part of interface from Director.
=======
The reason for implement as private inheritance is to hide some interface call by Director.
>>>>>>> a649a1b4fa488507f89f8891a6fda328a76e8584
*/
class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
<<<<<<< HEAD
    @brief  Called when the application moves to the background
=======
    @brief  The function be called when the application enter background
>>>>>>> a649a1b4fa488507f89f8891a6fda328a76e8584
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
<<<<<<< HEAD
    @brief  Called when the application reenters the foreground
=======
    @brief  The function be called when the application enter foreground
>>>>>>> a649a1b4fa488507f89f8891a6fda328a76e8584
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_

