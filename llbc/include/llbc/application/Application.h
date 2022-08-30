// The MIT License (MIT)

// Copyright (c) 2013 lailongwei<lailongwei@126.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), to deal in 
// the Software without restriction, including without limitation the rights to 
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of 
// the Software, and to permit persons to whom the Software is furnished to do so, 
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all 
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef __LLBC_APP_IAPPLICATION_H__
#define __LLBC_APP_IAPPLICATION_H__

#include "llbc/common/Common.h"
#include "llbc/core/Core.h"
#include "llbc/comm/Comm.h"

__LLBC_NS_BEGIN

/**
 * Pre-declare some classes.
 */
class LLBC_Packet;
class LLBC_IComponent;
class LLBC_IService;

__LLBC_NS_END

__LLBC_NS_BEGIN

/**
 * \brief The application config type enumeration.
 */
class LLBC_EXPORT LLBC_ApplicationConfigType
{
public:
    enum ENUM
    {
        Begin = 0,
        Ini = Begin,
        Xml,
        Property,

        End
    };

    /**
     * Get appliation config suffix.
     * @param[in] cfgType - the application config type.
     * @return const LLBC_String & - the config suffix.
     */
    static const LLBC_String &GetConfigSuffix(int cfgType);

    /**
     * Get config type. 
     * @param cfgSuffix - the config suffix, case insensitive.
     * @return ENUM - the config type, if unsupported application config, return End.
     */
    static ENUM GetConfigType(const LLBC_String &cfgSuffix);
};

/**
 * \brief The application interface class encapsulation.
 *        Note: Please call Start/Wait/Stop method at main thread.
 */
class LLBC_EXPORT LLBC_Application
{
public:
    LLBC_Application();
    virtual ~LLBC_Application();

public:
    /**
     * Application start event method, please override this method in your project.
     * @param[in] argc           - the application startup arguments count.
     * @param[in] argv           - the application startup arguments.
     * @param[out] startFinished - if startup finished set true, otherwise set false, default is true.
     * @return int - return 0 if start success, otherwise return -1.
     */
    virtual int OnStart(int argc, char *argv[], bool &startFinished) = 0;

    /**
     * Application stop event method, please override this method in your project.
     * @return bool - return true if stop finished, otherwise return false.
     */
    virtual bool OnStop() = 0;

    /**
     * Application config reloaded event method, please override this method in your project.
     */
    virtual void OnConfigReload();

public:
    /**
     * Get this application.
     * @return App * - this application.
     */
    template <typename App>
    static App *ThisApp();
    static LLBC_Application *ThisApp();

public:
    /**
     * Check have application config or not.
     * @return bool - return true if has config, otherwise return false.
     */
    bool HasConfig() const;

    /**
     * Get property type config.
     * @return const LLBC_Property & - the property config.
     */
    const LLBC_Property &GetPropertyConfig() const;

    /**
     * Get non-property type config.
     * @return const LLBC_Variant & - the non-property application config.
     */
    const LLBC_Variant &GetNonPropertyConfig() const;

    /**
     * Get application config type.
     * @return LLBC_ApplicationConfigType::ENUM - application config type.
     */
    LLBC_ApplicationConfigType::ENUM GetConfigType() const;

    /**
     * Get application config path.
     * @return const LLBC_String & - the application config path.
     */
    const LLBC_String &GetConfigPath() const;

    /**
     * Set application config path.
     * @param[in] cfgPath - the config path.
     * @return int - return 0 if success, other return -1.
     */
    int SetConfigPath(const LLBC_String &cfgPath);

    /**
     * Reload application config.
     * @param[in] callEvMeth - specific call event method when reload success or not.
     * @return int - return 0 if success, otherwise return -1.
     */
    int ReloadConfig(bool callEvMeth = true);

public:
    /**
     * Start application.
     * @param[in] name - the application name.
     * @param[in] argv - the application startup arguments.
     * @return int - return 0 if start success, otherwise return -1.
     */
    int Start(const LLBC_String &name, int argc, char *argv[]);

    /**
     * Stop application.
     */
    void Stop();

    /**
     * Check application started or not.
     */
    bool IsStarted() const;

public:
    /**
     * Set dump file when application dump.
     * @param[in] dumpFilePath - the dump file path.
     * @return int - return 0 if success, otherwise return -1.
     */
    int SetDumpFile(const LLBC_String &dumpFilePath);

    /**
     * Set crash hook, invoke after crashed.
     * @param[in] crashHook - the crash hook.
     * @return int - return 0 if success, otherwise return -1.
     */
    int SetCrashHook(const LLBC_Delegate<void(const LLBC_String &)> &crashHook);

public:
    /**
     * Get application name.
     * @return const LLBC_String & - application name.
     */
    const LLBC_String &GetName() const;

    /**
     * Get startup arguments.
     * @return const LLBC_StartArgs & - the startup arguments(llbc library wrapped object const reference).
     */
    const LLBC_StartArgs &GetStartArgs() const;

public:
    /**
     * Get service.
     * @param[in] id - service Id.
     * @return LLBC_IService * - service.
     */
    LLBC_IService *GetService(int id) const;

    /**
     * Remove service.
     * @param[in] id - service Id.
     * @return int - return 0 if success, otherwise return -1.
     */
    int RemoveService(int id);

private:
    /**
     * Locate application config path.
     * @param[in] appName  - the application name.
     * @param[out] cfgType - the application config type.
     * @return LLBC_String - the application config path, return enpty string if failed.
     */
    static LLBC_String LocateConfigPath(const LLBC_String &appName, int &cfgType);

    /**
     * Reload application config.
     * @return int - return 0 if success, otherwise return -1.
     */
    int LoadConfig();
    int LoadIniConfig();
    int LoadXmlConfig();
    int LoadPropertyConfig();

protected:
    LLBC_String _name;
    LLBC_SpinLock _cfgLock;

    bool _llbcLibStartupInApp;

    volatile bool _loadingCfg;
    LLBC_Property _propCfg;
    LLBC_Variant _nonPropCfg;
    LLBC_String _cfgPath;
    LLBC_ApplicationConfigType::ENUM _cfgType;

    LLBC_ServiceMgr &_services;

private:
    volatile bool _started;
    LLBC_StartArgs _startArgs;

#if LLBC_TARGET_PLATFORM_WIN32
    LLBC_String _dumpFilePath;
    LLBC_Delegate<void(const LLBC_String &)> _crashHook;
#endif // Win32

    static LLBC_Application *_thisApp;
};

__LLBC_NS_END

#include "llbc/application/ApplicationImpl.h"

#endif // !__LLBC_APP_IAPPLICATION_H__