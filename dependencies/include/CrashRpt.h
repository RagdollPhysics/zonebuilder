/************************************************************************************* 
  This file is a part of CrashRpt library.

  Copyright (c) 2003, Michael Carruth
  All rights reserved.
 
  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:
 
   * Redistributions of source code must retain the above copyright notice, this 
     list of conditions and the following disclaimer.
 
   * Redistributions in binary form must reproduce the above copyright notice, 
     this list of conditions and the following disclaimer in the documentation 
     and/or other materials provided with the distribution.
 
   * Neither the name of the author nor the names of its contributors 
     may be used to endorse or promote products derived from this software without 
     specific prior written permission.
 

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************************************************************************************/

/*! \file   CrashRpt.h
 *  \brief  Defines the interface for the CrashRpt.DLL.
 *  \date   2003-2010
 *  \author Michael Carruth 
 *  \author zeXspectrum 
 */

#ifndef _CRASHRPT_H_
#define _CRASHRPT_H_

#include <windows.h>
#include <dbghelp.h>

// Define SAL macros to be empty if some old Visual Studio used
#ifndef __reserved
  #define __reserved
#endif
#ifndef __in
  #define __in
#endif
#ifndef __in_opt
  #define __in_opt
#endif
#ifndef __out_ecount_z
  #define __out_ecount_z(x)
#endif

#ifdef __cplusplus
#define CRASHRPT_EXTERNC extern "C"
#else
#define CRASHRPT_EXTERNC
#endif

#define CRASHRPTAPI(rettype) CRASHRPT_EXTERNC rettype WINAPI

//! Current CrashRpt version
#define CRASHRPT_VER 1207

/*! \defgroup CrashRptAPI CrashRpt Functions */
/*! \defgroup DeprecatedAPI Obsolete Functions */
/*! \defgroup CrashRptStructs CrashRpt Structures */
/*! \defgroup CrashRptWrappers CrashRpt Wrapper Classes */

/*! \ingroup CrashRptAPI
 *  \brief Client crash callback function prototype
 *  \param[in] lpvState Not used, always equals to NULL.
 *
 *  \remarks
 *  The crash callback function is called when crash occurs. This way client application is
 *  notified about the crash.
 *
 *  It is generally unsafe to do complex actions (e.g. memory allocation, heap operations) inside of this callback.
 *  The application state may be unstable.
 *  
 *  The crash callback function should return \c TRUE to allow generate error report. It should 
 *  return \c FALSE to prevent crash report generation.
 *
 *  The following example shows how to use crash callback function.
 *
 *  \code
 *  // define the crash callback
 *  BOOL CALLBACK CrashCallback(LPVOID lpvState)
 *  {    
 *    // add custom log file to crash report
 *    crAddFile2(
 *       _T("C:\\Documents and Settings\\Application Data\\UserName\\MyApp\\Logs\\MyLog.txt"), 
 *       NULL,
 *       _T("My custom log file"),
 *       0);
 *
 *    return TRUE;
 *  }
 *
 *  \endcode
 */
typedef BOOL (CALLBACK *LPGETLOGFILE) (__reserved LPVOID lpvState);

#ifndef _CRASHRPT_REMOVE_DEPRECATED

/*! \ingroup DeprecatedAPI
 *  \brief Installs exception handlers for the current process.
 *
 *  \param[in] pfnCallback Client crash callback.
 *  \param[in] pszEmailTo Email address to send crash report to.
 *  \param[in] pszEmailSubject Subject of the E-mail message.
 *
 *  \return Always returns NULL.
 *
 *  \deprecated
 *    This function is deprecated. It is still supported for compatiblity with
 *    older versions of CrashRpt, however consider using crInstall() function instead.
 *    This function is currently implemented as a wrapper for crInstall().    
 *
 *  \remarks
 *
 *    This function installs unhandled exception filter for all threads of calling process.
 *    It also installs various C++ exception/error handlers. For the list of handlers,
 *    please see crInstall().
 *
 *    \a pfnCallback defines the callback function that is called on crash. The callback can be
 *    used to add a custom file to crash report using AddFile() function. This parameter can be NULL.
 *
 *    \a pszEmailTo should be the valid email address of recipient. 
 *
 *    \a pszEmailSubject is the email subject. If this parameter is NULL, the default subject is 
 *    generated.
 *
 *    This function assumes that CrashSender.exe is located in the same directory as CrashRpt.dll loaded
 *    by the caller process. To specify different directory, use crInstall().
 *
 *    On crash, the error report is sent as E-mail message using address and subject passed to the
 *    function as \a pszEmailTo and \a pszEmailSubject parameters, respectively.
 *    
 *    InstallW() and InstallA() are wide-character and multibyte-character versions of Install(). 
 *    The Install() macro defines character set independent mapping for these functions. 
 * 
 */

CRASHRPTAPI(LPVOID)
InstallW(
   __in_opt LPGETLOGFILE pfnCallback,
   LPCWSTR pszEmailTo,    
   LPCWSTR pszEmailSubject
   );

/*! \ingroup DeprecatedAPI
 *  \copydoc InstallW()
 */

CRASHRPTAPI(LPVOID)
InstallA(
   __in_opt LPGETLOGFILE pfnCallback,
   LPCSTR pszEmailTo,    
   LPCSTR pszEmailSubject
   );

/*! \brief Character set-independent mapping of InstallW() and InstallA() functions. 
 *  \ingroup DeprecatedAPI
 */
#ifdef UNICODE
#define Install InstallW 
#else
#define Install InstallA
#endif //UNICODE


/*! \ingroup DeprecatedAPI
 *  \brief Uninstalls the exception filters set up by Install().
 *
 *  \param[in] lpState State information returned from Install(), ignored and should be NULL.
 *
 *  \deprecated
 *    This function is deprecated. It is still supported for compatiblity with
 *    older versions of CrashRpt, however consider using crUninstall() function instead.
 *    This function is implemented as a wrapper for crUninstall().
  *
 *  \remarks
 *
 *    Call this function on application exit to uninstall all previously installed exception
 *    handlers.
 *
 *    The \a lpState parameter is unused and should be NULL.
 */

CRASHRPTAPI(void)
Uninstall(
   __reserved LPVOID lpState                            
   );

/*! \ingroup DeprecatedAPI 
 *  \brief Adds a file to the crash report.
 *  
 *  \param[in] lpState State information returned from Install(), ignored and should be NULL.
 *  \param[in] pszFile  Fully qualified file name.
 *  \param[in] pszDesc  Description of file, used by Error Report Details dialog.
 *
 *  \deprecated
 *    This function is deprecated. It is still supported for compatiblity with
 *    older versions of CrashRpt, however consider using crAddFile2() function instead.
 *    This function is implemented as a wrapper for crAddFile2().
 *
 *  \remarks
 *
 *    This function can be called anytime after Install() to add one or more
 *    files to the generated crash report. However, the recommended way is to 
 *    call this function in crash callback.
 *
 *    \a pszFile should be a valid absolute path of a file to add to crash report. It
 *    is recommended to add small files (several KB in size). If a large file is added,
 *    the crash report sending procedure may fail.
 *
 *    \a pszDesc is a description of a file. It can be NULL.
 *
 *    Function fails if \a pszFile doesn't exist at the moment of function call.
 *
 *    AddFileW() and AddFileA() are wide-character and multibyte-character versions of AddFile(). 
 *    The AddFile() macro defines character set independent mapping for these functions. 
 *
 */

CRASHRPTAPI(void)
AddFileW(
   __reserved LPVOID lpState,                         
   LPCWSTR pszFile,                         
   LPCWSTR pszDesc                          
   );

/*! \ingroup DeprecatedAPI
 *
 *  \copydoc AddFileW()
 */

CRASHRPTAPI(void)
AddFileA(
   __reserved LPVOID lpState,                         
   LPCSTR pszFile,                         
   LPCSTR pszDesc                          
   );

/*! \brief Character set-independent mapping of AddFileW() and AddFileA() functions. 
 *  \ingroup DeprecatedAPI
 */
#ifdef UNICODE
#define AddFile AddFileW
#else
#define AddFile AddFileA
#endif //UNICODE



/*! \ingroup DeprecatedAPI 
 *  \brief Generates the crash report.
 *  
 *  \param[in] lpState     State information returned from Install(), ignored and should be NULL.
 *  \param[in] pExInfo     Pointer to an EXCEPTION_POINTERS structure, can be NULL.
 * 
 *  \deprecated
 *    This function is deprecated. It is still supported for compatiblity with
 *    older versions of CrashRpt, however consider using crGenerateErrorReport() function instead.
 *    This function is implemented as a wrapper for crGenerateErrorReport().
 *
 *  \remarks
 *
 *    Call this function to manually generate a crash report.
 *
 *    The crash report contains the crash minidump, crash log in XML format and
 *    additional optional files added with AddFile().
 *
 *    \a pExInfo defines the exception pointers for generating crash minidump file.
 *    If \a pExInfo is NULL, current CPU state is used to create exception pointers.
 *
 *    This function generates the error report and returns control to the caller. It doesn't
 *    terminate the caller process.
 *
 */

CRASHRPTAPI(void)
GenerateErrorReport(
   __reserved LPVOID lpState,
   PEXCEPTION_POINTERS pExInfo
   );

#endif //_CRASHRPT_REMOVE_DEPRECATED

// Array indices for CR_INSTALL_INFO::uPriorities.
#define CR_HTTP 0 //!< Send error report via HTTP connection.
#define CR_SMTP 1 //!< Send error report via SMTP connection.
#define CR_SMAPI 2 //!< Send error report via simple MAPI (using default mail client).

//! Special priority constant that allows to skip certain delivery method.
#define CR_NEGATIVE_PRIORITY ((UINT)-1)

// Flags for CR_INSTALL_INFO::dwFlags
#define CR_INST_STRUCTURED_EXCEPTION_HANDLER   0x1    //!< Install SEH handler (deprecated name, use \ref CR_INST_SEH_EXCEPTION_HANDLER instead.
#define CR_INST_SEH_EXCEPTION_HANDLER   0x1           //!< Install SEH handler.
#define CR_INST_TERMINATE_HANDLER              0x2    //!< Install terminate handler.
#define CR_INST_UNEXPECTED_HANDLER             0x4    //!< Install unexpected handler.
#define CR_INST_PURE_CALL_HANDLER              0x8    //!< Install pure call handler (VS .NET and later).
#define CR_INST_NEW_OPERATOR_ERROR_HANDLER     0x10   //!< Install new operator error handler (VS .NET and later).
#define CR_INST_SECURITY_ERROR_HANDLER         0x20   //!< Install security errror handler (VS .NET and later).
#define CR_INST_INVALID_PARAMETER_HANDLER      0x40   //!< Install invalid parameter handler (VS 2005 and later).
#define CR_INST_SIGABRT_HANDLER                0x80   //!< Install SIGABRT signal handler.
#define CR_INST_SIGFPE_HANDLER                 0x100  //!< Install SIGFPE signal handler.   
#define CR_INST_SIGILL_HANDLER                 0x200  //!< Install SIGILL signal handler.  
#define CR_INST_SIGINT_HANDLER                 0x400  //!< Install SIGINT signal handler.  
#define CR_INST_SIGSEGV_HANDLER                0x800  //!< Install SIGSEGV signal handler.
#define CR_INST_SIGTERM_HANDLER                0x1000 //!< Install SIGTERM signal handler.  

#define CR_INST_ALL_EXCEPTION_HANDLERS 0      //!< Install all possible exception handlers.
#define CR_INST_CRT_EXCEPTION_HANDLERS 0x1FFE //!< Install exception handlers for the linked CRT module.

#define CR_INST_NO_GUI                         0x2000 //!< Do not show GUI, send report silently (use for non-GUI apps only).
#define CR_INST_HTTP_BINARY_ENCODING           0x4000 //!< Use multi-part HTTP uploads with binary attachment encoding.
#define CR_INST_DONT_SEND_REPORT               0x8000 //!< Don't send error report immediately, just save it locally.
#define CR_INST_APP_RESTART                    0x10000 //!< Restart the application on crash.
#define CR_INST_NO_MINIDUMP                    0x20000 //!< Do not include minidump file to crash report.
#define CR_INST_SEND_QUEUED_REPORTS            0x40000 //!< CrashRpt should send error reports that are waiting to be delivered.
#define CR_INST_STORE_ZIP_ARCHIVES             0x80000 //!< CrashRpt should store both uncompressed error report files and ZIP archives.

/*! \ingroup CrashRptStructs
 *  \struct CR_INSTALL_INFOW()
 *  \brief This structure defines the general information used by crInstall()
 *
 *  \remarks
 *
 *    \a cb should always contain the size of this structure in bytes. 
 *
 *    \a pszAppName is the friendly name of the client application. The application name is
 *       displayed in Error Report dialog. If this parameter is NULL, the name of EXE file that was used to start caller
 *       process becomes the application name.
 *
 *    \a pszAppVersion should be the application version. Example: "1.0.1". 
 *       If it equals to NULL, product version is extracted from the executable file which started 
 *       the process and this product version is used as application version.
 * 
 *    \a pszEmailTo is the email address of the recipient of error reports, for example
 *       "name@example.com". If this equals to NULL, the crash report won't be sent using E-mail client.
 *       Keep this NULL if you plan to use large error reports (more than several MB in size), because
 *       large emails may be rejected by the mail server.
 *       To define a custom port for SMTP connection, use the following address format: "user@example.com:port",
 *       where \a port is the placeholder for the port number. 
 *
 *    \a pszEmailSubject is the subject of the email message. If this parameter is NULL,
 *       the default subject of form '[app_name] [app_version] Error Report' is generated.
 *
 *    \a pszUrl is the URL of a server-side script that would receive crash report data via HTTP
 *       connection. If this parmeter is NULL, HTTP connection won't be used to send crash reports. For
 *       example of a server-side script that can receive crash report, see \ref sending_error_reports.
 *       HTTP transport is the recommended way of sending large error reports (more than several MB in size).
 *       To define a custom port for HTTP connection, use the following URL format: "http://example.com:port/crashrpt.php",
 *       where \a port is the placeholder for the port number.
 *
 *    \a pszCrashSenderPath is the absolute path to the directory where CrashSender.exe is located. 
 *       The crash sender process is responsible for letting end user know about the crash and 
 *       sending the error report.
 *       If this is NULL, it is assumed that CrashSender.exe is located in
 *       the same directory as CrashRpt.dll.
 *
 *    \a pfnCrashCallback is a pointer to the LPGETLOGFILE() crash callback function. The crash callback function is
 *         called by CrashRpt when crash occurs and allows user to be notified.
 *         If this is NULL, crash callback is not called.
 *
 *    \a uPriorities is an array that defines the preferred ways of sending error reports. 
 *         The available ways are: HTTP connection, SMTP connection or simple MAPI (default mail client).
 *         A priority is a non-negative integer number or special constant \ref CR_NEGATIVE_PRIORITY. The greater positive number defines the greater priority. 
 *         Specify the \ref CR_NEGATIVE_PRIORITY to skip the given way.
 *         The element having index \ref CR_HTTP defines priority for using HTML connection.
 *         The element having index \ref CR_SMTP defines priority for using SMTP connection.
 *         The element having index \ref CR_SMAPI defines priority for using the default mail client.
 *         The ways having greater priority will be tried first. If priorities are equal to each other, HTML
 *         connection will be tried the first, SMTP connection will be tried the second and simple MAPI will be tried
 *         the last. 
 *
 *    <b>Since v1.1.2</b> \a dwFlags can be used to define some behavior parameters. This can be a combination of the following values:
 *
 *    <table>
 *    <tr><td colspan="2"> <i>Use the combination of the following constants to specify what exception handlers to install:</i>
 *    <tr><td> \ref CR_INST_ALL_EXCEPTION_HANDLERS    <td> Install all available exception handlers.
 *    <tr><td> \ref CR_INST_SEH_EXCEPTION_HANDLER     <td> Install SEH exception handler.
 *    <tr><td> \ref CR_INST_PURE_CALL_HANDLER         <td> Install pure call handler (VS .NET and later).
 *    <tr><td> \ref CR_INST_NEW_OPERATOR_ERROR_HANDLER <td> Install new operator error handler (VS .NET and later).
 *    <tr><td> \ref CR_INST_SECURITY_ERROR_HANDLER     <td> Install security errror handler (VS .NET and later).
 *    <tr><td> \ref CR_INST_INVALID_PARAMETER_HANDLER  <td> Install invalid parameter handler (VS 2005 and later).
 *    <tr><td> \ref CR_INST_SIGABRT_HANDLER            <td> Install SIGABRT signal handler.
 *    <tr><td> \ref CR_INST_SIGINT_HANDLER             <td> Install SIGINT signal handler.  
 *    <tr><td> \ref CR_INST_SIGTERM_HANDLER            <td> Install SIGTERM signal handler.  
 *    <tr><td colspan="2"> <i>Use the combination of the following constant to define behavior parameters:</i>
 *    <tr><td> \ref CR_INST_NO_GUI                
 *        <td> <b>Available since v.1.2.2</b> Do not show GUI.
 * 
 *             It is not recommended to use this flag for regular GUI-based applications. 
 *             Use this only for services that have no GUI.
 *    <tr><td> \ref CR_INST_HTTP_BINARY_ENCODING     
 *        <td> <b>Available since v.1.2.2</b> This affects the way of sending reports over HTTP. 
 *             By specifying this flag, you enable usage of multi-part HTTP uploads with binary encoding instead of the legacy way (Base64-encoded form data). 
 *
 *             It is recommended to always specify this flag, because it is more suitable for large error reports. The legacy way
 *             is supported for backwards compatibility and not recommended to use.
 *             For additional information, see \ref sending_error_reports.
 *    <tr><td> \ref CR_INST_DONT_SEND_REPORT     
 *        <td> <b>Available since v.1.2.2</b> This parameter means 'do not send error report immediately on crash, just save it locally'. 
 *             Use this if you have direct access to the machine where crash happens and do not need 
 *             to send report over the Internet. You can use this in couple with \ref CR_INST_STORE_ZIP_ARCHIVES flag to store zipped error reports
 *             along with uncompressed error report files.
 *    <tr><td> \ref CR_INST_APP_RESTART     
 *        <td> <b>Available since v.1.2.4</b> This parameter allows to automatically restart the application on crash. The command line
 *             for the application is taken from \a pszRestartCmdLine parameter. To avoid cyclic restarts of an application which crashes on startup, 
 *             the application is restarted only if at least 60 seconds elapsed since its start.
 *    <tr><td> \ref CR_INST_NO_MINIDUMP     
 *        <td> <b>Available since v.1.2.4</b> Specify this parameter if you want minidump file not to be included into crash report. The default
 *             behavior is to include the minidump file.
 *
 *    <tr><td> \ref CR_INST_SEND_QUEUED_REPORTS     
 *        <td> <b>Available since v.1.2.5</b> Specify this parameter to send all queued reports. Those
 *             report files are by default stored in <i>%LOCAL_APPDATA%\\CrashRpt\\UnsentCrashReports\\%AppName%_%AppVersion%</i> folder.
 *             If this is specified, CrashRpt checks if it's time to remind user about recent errors in the application and offers to send
 *             all queued error reports.
 *
 *    <tr><td> \ref CR_INST_STORE_ZIP_ARCHIVES     
 *        <td> <b>Available since v.1.2.7</b> This parameter can be used in couple with \ref CR_INST_DONT_SEND_REPORT flag to store not only uncompressed
 *             error report files, but also ZIP archives. By default (if this flag omitted) CrashRpt stores all error report files
 *             in uncompressed state.
 *
 *   </table>
 *
 *   <b>Since v1.1.2</b>, \a pszPrivacyPolicyURL defines the URL for the Privacy Policy hyperlink of the 
 *   Error Report dialog. If this parameter is NULL, the link is not displayed. For information on the Privacy Policy, see \ref error_report.
 *
 *   <b>Since v1.2.1</b>, \a pszDebugHelpDLL parameter defines the location of the dbghelp.dll to load. 
 *   If this parameter is NULL, the dbghelp.dll is searched using the default search sequence.
 *
 *   <b>Since v.1.2.1</b>, \a uMiniDumpType parameter defines the minidump type. For the list of available minidump
 *   types, see the documentation for <b>MiniDumpWriteDump()</b> function in MSDN. It is recommended to set this 
 *   parameter with zero (equivalent of \b MiniDumpNormal constant). Other values may increase the minidump size significantly.
 *   If you plan to use values other than zero, also specify the \ref CR_INST_HTTP_BINARY_ENCODING flag for \a dwFlags parameter.
 *
 *   <b>Since v.1.2.2</b>, \a pszErrorReportSaveDir parameter defines the directory where to save the error reports. 
 *   If this is NULL, the default directory is used (%%LOCAL_APP_DATA%\\CrashRpt\\UnsentCrashReports\\%%AppName%%_%%AppVersion%).
 *
 *   <b> Since v.1.2.4</b>, \a pszRestartCmdLine parameter defines the string that specifies the 
 *   command-line arguments for the application when it is restarted (when using \ref CR_INST_APP_RESTART flag). Do not include the name of 
 *   the executable in the command line; it is added automatically. This parameter can be NULL.
 *
 *   <b> Since v.1.2.4</b>, \a pszLangFilePath parameter defines the absolute path (including file name) for language file.
 *   If this is NULL, the lang file is assumed to be located in the same dir as \b CrashSender.exe file and have the name \b crashrpt_lang.ini.
 * 
 *   <b> Since v.1.2.4</b>, \a pszEmailText parameter defines the custom E-mail text that is used when deliverying error report
 *   as E-mail. If this is NULL, the default E-mail text is used. It is recommended to set this parameter with NULL.
 *  
 *   <b> Since v.1.2.4</b>, \a pszSmtpProxy parameter defines the network address and port formatted as "IP:port" to use as SMTP proxy. Example: "192.168.1.1:2525".
 *   If this is NULL, the SMTP server address is resolved using the MX record of sender's or recipient's mailbox. You should typically set this parameter with NULL, except in the
 *   case when your software is a server and custom SMTP configuration is required.
 *  
 *  \note
 *
 *    \ref CR_INSTALL_INFOW and \ref CR_INSTALL_INFOA structures are wide-character and multi-byte character 
 *    versions of \ref CR_INSTALL_INFO. \ref CR_INSTALL_INFO typedef defines character set independent mapping.
 *        
 */

typedef struct tagCR_INSTALL_INFOW
{
  WORD cb;                        //!< Size of this structure in bytes; must be initialized before using!
  LPCWSTR pszAppName;             //!< Name of application.
  LPCWSTR pszAppVersion;          //!< Application version.
  LPCWSTR pszEmailTo;             //!< E-mail address of crash reports recipient.
  LPCWSTR pszEmailSubject;        //!< Subject of crash report e-mail. 
  LPCWSTR pszUrl;                 //!< URL of server-side script (used in HTTP connection).
  LPCWSTR pszCrashSenderPath;     //!< Directory name where CrashSender.exe is located.
  LPGETLOGFILE pfnCrashCallback;  //!< User crash callback.
  UINT uPriorities[5];            //!< Array of error sending transport priorities.
  DWORD dwFlags;                  //!< Flags.
  LPCWSTR pszPrivacyPolicyURL;    //!< URL of privacy policy agreement.
  LPCWSTR pszDebugHelpDLL;        //!< File name or folder of Debug help DLL.
  MINIDUMP_TYPE uMiniDumpType;    //!< Minidump type.
  LPCWSTR pszErrorReportSaveDir;  //!< Directory where to save error reports.
  LPCWSTR pszRestartCmdLine;      //!< Command line for application restart (without executable name).
  LPCWSTR pszLangFilePath;        //!< Path to the language file (including file name).
  LPCWSTR pszEmailText;           //!< Custom E-mail text (used when deliverying report as E-mail).
  LPCWSTR pszSmtpProxy;           //!< Network address and port to be used as SMTP proxy.
}
CR_INSTALL_INFOW;

typedef CR_INSTALL_INFOW *PCR_INSTALL_INFOW;

/*! \ingroup CrashRptStructs
 *  \struct CR_INSTALL_INFOA
 *  \copydoc CR_INSTALL_INFOW
 */

typedef struct tagCR_INSTALL_INFOA
{
  WORD cb;                       //!< Size of this structure in bytes; must be initialized before using!
  LPCSTR pszAppName;             //!< Name of application.
  LPCSTR pszAppVersion;          //!< Application version.
  LPCSTR pszEmailTo;             //!< E-mail address of crash reports recipient.
  LPCSTR pszEmailSubject;        //!< Subject of crash report e-mail. 
  LPCSTR pszUrl;                 //!< URL of server-side script (used in HTTP connection).
  LPCSTR pszCrashSenderPath;     //!< Directory name where CrashSender.exe is located.
  LPGETLOGFILE pfnCrashCallback; //!< User crash callback.
  UINT uPriorities[3];           //!< Array of error sending transport priorities.
  DWORD dwFlags;                 //!< Flags.
  LPCSTR pszPrivacyPolicyURL;    //!< URL of privacy policy agreement.
  LPCSTR pszDebugHelpDLL;        //!< File name or folder of Debug help DLL.
  MINIDUMP_TYPE uMiniDumpType;   //!< Mini dump type.
  LPCSTR pszErrorReportSaveDir;  //!< Directory where to save error reports.
  LPCSTR pszRestartCmdLine;      //!< Command line for application restart (without executable name).
  LPCSTR pszLangFilePath;        //!< Path to the language file (including file name).
  LPCSTR pszEmailText;           //!< Custom E-mail text (used when deliverying report as E-mail).
  LPCSTR pszSmtpProxy;           //!< Network address and port to be used as SMTP proxy.
}
CR_INSTALL_INFOA;

typedef CR_INSTALL_INFOA *PCR_INSTALL_INFOA;

/*! \brief Character set-independent mapping of CR_INSTALL_INFOW and CR_INSTALL_INFOA structures.
 *  \ingroup CrashRptStructs
 */
#ifdef UNICODE
typedef CR_INSTALL_INFOW CR_INSTALL_INFO;
typedef PCR_INSTALL_INFOW PCR_INSTALL_INFO;
#else
typedef CR_INSTALL_INFOA CR_INSTALL_INFO;
typedef PCR_INSTALL_INFOA PCR_INSTALL_INFO; 
#endif // UNICODE

/*! \ingroup CrashRptAPI 
 *  \brief  Installs exception handlers for all threads of the caller process.
 *
 *  \param[in] pInfo General information.
 *
 *  \remarks
 *    This function installs unhandled exception filter for all threads of the caller process.
 *    It also installs various CRT exception/error handlers that function for all threads of the caller process.
 *    For more information, see \ref exception_handling
 *
 *    Below is the list of installed handlers:
 *     - Top-level SEH exception filter [ \c SetUnhandledExceptionFilter() ]
 *     - C++ pure virtual call handler (Visual Studio .NET 2003 and later) [ \c _set_purecall_handler() ]
 *     - C++ invalid parameter handler (Visual Studio .NET 2005 and later) [ \c _set_invalid_parameter_handler() ]
 *     - C++ new operator error handler (Visual Studio .NET 2003 and later) [ \c _set_new_handler() ]
 *     - C++ buffer overrun handler (Visual Studio .NET 2003 only) [ \c _set_security_error_handler() ]
 *     - C++ abort handler [ \c signal(SIGABRT) ]
 *     - C++ illegal instruction handler [ \c signal(SIGINT) ]
 *     - C++ termination request [ \c signal(SIGTERM) ]
 *
 *    In a multithreaded program, additionally use crInstallToCurrentThread2() function for each execution
 *    thread, except the main one.
 * 
 *    The \a pInfo parameter contains all required information needed to install CrashRpt.
 *
 *    This function fails when \a pInfo->pszCrashSenderPath doesn't contain valid path to CrashSender.exe
 *    or when \a pInfo->pszCrashSenderPath is equal to NULL, but CrashSender.exe is not located in the
 *    directory where CrashRpt.dll located.
 *
 *    On crash, the crash minidump file is created, which contains CPU and 
 *    stack state information. Also XML file is created that contains additional 
 *    information that may be helpful for crash analysis. These files along with several additional
 *    files added with crAddFile2() are packed to a single ZIP file.
 *
 *    When crash information is collected, another process, <b>CrashSender.exe</b>, is launched 
 *    and the process where crash had occured is terminated. The CrashSender process is 
 *    responsible for letting the user know about the crash and send the error report.
 * 
 *    The error report can be sent over E-mail using address and subject passed to the
 *    function as \ref CR_INSTALL_INFO structure members. Another way of sending error report is an HTTP 
 *    request using \a pszUrl member of \ref CR_INSTALL_INFO. If both the E-mail address and
 *    URL are not specified, this function fails.
 *
 *    This function may fail if an appropriate language file (\b crashrpt_lang.ini) is not found 
 *    in the directory where the \b CrashSender.exe file is located.
 *
 *    If this function fails, use crGetLastErrorMsg() to retrieve the error message.
 *
 *    crInstallW() and crInstallA() are wide-character and multi-byte character versions of crInstall()
 *    function. The \ref crInstall macro defines character set independent mapping for these functions.
 *
 *    For code example, see \ref simple_example.
 *
 *  \sa crInstallW(), crInstallA(), crInstall(), CR_INSTALL_INFOW, 
 *      CR_INSTALL_INFOA, CR_INSTALL_INFO, crUninstall(), 
 *      CrAutoInstallHelper
 */

CRASHRPTAPI(int)
crInstallW(
  __in PCR_INSTALL_INFOW pInfo
);

/*! \ingroup CrashRptAPI
 *  \copydoc crInstallW()
 */

CRASHRPTAPI(int)
crInstallA(
  __in PCR_INSTALL_INFOA pInfo
);

/*! \brief Character set-independent mapping of crInstallW() and crInstallA() functions. 
 * \ingroup CrashRptAPI
 */
#ifdef UNICODE
#define crInstall crInstallW
#else
#define crInstall crInstallA
#endif //UNICODE

/*! \ingroup CrashRptAPI 
 *  \brief Unsinstalls exception handlers previously installed with crInstall().
 *
 *  \return
 *    This function returns zero if succeeded.
 *
 *  \remarks
 *
 *    Call this function on application exit to uninstall exception
 *    handlers previously installed with crInstall(). After function call, the exception handlers
 *    are restored to states they had before calling crInstall().
 *
 *    This function fails if crInstall() wasn't previously called in context of
 *    current process.
 *
 *    When this function fails, use crGetLastErrorMsg() to retrieve the error message.
 *
 *  \sa crInstallW(), crInstallA(), crInstall(), crUninstall(),
 *      CrAutoInstallHelper
 */

CRASHRPTAPI(int)
crUninstall();


/*! \ingroup DeprecatedAPI  
 *  \brief Installs exception handlers to the current thread.
 *
 *  \return This function returns zero if succeeded.
 *   
 *  \remarks
 *   
 *   This function sets exception handlers for the caller thread. If you have
 *   several execution threads, you ought to call the function for each thread,
 *   except the main one.
 *  
 *   The list of C++ exception/error handlers installed with this function:
 *    - terminate handler [ \c set_terminate() ]
 *    - unexpected handler [ \c set_unexpected() ]
 *    - floating point error handler [ \c signal(SIGFPE) ]
 *    - illegal instruction handler [ \c signal(SIGILL) ]
 *    - illegal storage access handler [ \c signal(SIGSEGV) ]    
 *
 *   The crInstall() function automatically installs C++ exception handlers for the
 *   main thread, so no need to call crInstallToCurrentThread() for the main thread.
 *
 *   This function fails if calling it twice for the same thread.
 *   When this function fails, use crGetLastErrorMsg() to retrieve the error message.
 * 
 *   Call crUninstallFromCurrentThread() to uninstall C++ exception handlers from
 *   current thread.
 *
 *   The crInstallToCurrentThread2() function gives better control of what exception 
 *   handlers to install. 
 *
 *   The following example shows how to use crInstallToCurrentThread() and crUninstallFromCurrentThread().
 *
 *   \code
 *
 *   DWORD WINAPI ThreadProc(LPVOID lpParam)
 *   {
 *     // Install exception handlers
 *     crInstallToCurrentThread();
 *
 *     // Your code...
 *
 *     // Uninstall exception handlers
 *     crUninstallFromCurrentThread();
 *    
 *     return 0;
 *   }
 *
 *   // .. Create a thread
 *   DWORD dwThreadId = 0;
 *   HANDLE hWorkingThread = CreateThread(NULL, 0, 
 *            ThreadProc, (LPVOID)NULL, 0, &dwThreadId);
 *
 *   \endcode
 *
 *   \sa crInstallToCurrentThread(), crInstallToCurrentThread2(),
 *       crUninstallFromCurrentThread(), CrThreadAutoInstallHelper
 */

CRASHRPTAPI(int)
crInstallToCurrentThread();

/*! \ingroup CrashRptAPI
 *  \brief Installs exception handlers to the caller thread.
 *  \return This function returns zero if succeeded.
 *  \param[in] dwFlags Flags.
 *
 *  \remarks
 *
 *  This function is available <b>since v.1.1.2</b>.
 *
 *  The function sets exception handlers for the caller thread. If you have
 *  several execution threads, you ought to call the function for each thread,
 *  except the main one.
 *   
 *  The function works the same way as crInstallToCurrentThread(), but provides
 *  an ability to select what exception handlers to install.
 *
 *  \a dwFlags defines what exception handlers to install. Use zero value
 *  to install all possible exception
 *  handlers. Or use a combination of the following constants:
 *
 *      - \ref CR_INST_TERMINATE_HANDLER              Install terminate handler
 *      - \ref CR_INST_UNEXPECTED_HANDLER             Install unexpected handler
 *      - \ref CR_INST_SIGFPE_HANDLER                 Install SIGFPE signal handler   
 *      - \ref CR_INST_SIGILL_HANDLER                 Install SIGILL signal handler  
 *      - \ref CR_INST_SIGSEGV_HANDLER                Install SIGSEGV signal handler 
 * 
 *  Example:
 *
 *   \code
 *   DWORD WINAPI ThreadProc(LPVOID lpParam)
 *   {
 *     // Install exception handlers
 *     crInstallToCurrentThread2(0);
 *
 *     // Your code...
 *
 *     // Uninstall exception handlers
 *     crUninstallFromCurrentThread();
 *    
 *     return 0;
 *   }
 *   \endcode
 * 
 *  \sa 
 *    crInstallToCurrentThread()
 */

CRASHRPTAPI(int)
crInstallToCurrentThread2(DWORD dwFlags);

/*! \ingroup CrashRptAPI  
 *  \brief Uninstalls C++ exception handlers from the current thread.
 *  \return This function returns zero if succeeded.
 *  
 *  \remarks
 *
 *    This function unsets C++ exception handlers for the caller thread. If you have
 *    several execution threads, you ought to call the function for each thread.
 *    After calling this function, the exception handlers for current thread are
 *    replaced with the handlers that were before call of crInstallToCurrentThread2() 
 *    (or crInstallToCurrentThread()).
 *
 *    This function fails if crInstallToCurrentThread() (or crInstallToCurrentThread2())
 *    wasn't called for current thread.
 *    When this function fails, use crGetLastErrorMsg() to retrieve the error message.
 *
 *    No need to call this function for the main execution thread. The crUninstall()
 *    will automatically uninstall C++ exception handlers for the main thread.
 *
 *   \sa crInstallToCurrentThread(), crInstallToCurrentThread2(),
 *       crUninstallFromCurrentThread(), CrThreadAutoInstallHelper
 */

CRASHRPTAPI(int)
crUninstallFromCurrentThread();

/*! \ingroup DeprecatedAPI  
 *  \brief Adds a file to crash report.
 * 
 *  \return This function returns zero if succeeded.
 *
 *  \param[in] pszFile Absolute path to the file to add.
 *  \param[in] pszDesc File description (used in Error Report Details dialog).
 *
 *  \note
 *    This function is deprecated. It is recommended to use crAddFile2() function instead.
 *
 *    This function can be called anytime after crInstall() to add one or more
 *    files to the generated crash report. 
 * 
 *    When this function is called, the file is marked to be added to the error report, 
 *    then the function returns control to the caller.
 *    When crash occurs, all marked files are added to the report. If a file is locked by someone 
 *    for exclusive access, the file won't be included. In \ref LPGETLOGFILE crash callback, 
 *    ensure files to be included are not locked.
 *  
 *    \a pszFile should be a valid absolute path of a file to add to crash report. 
 *
 *    \a pszDesc is a description of a file. It can be NULL.
 *
 *    Function fails if \a pszFile doesn't exist at the moment of function call. 
 * 
 *    The crAddFileW() and crAddFileA() are wide-character and multibyte-character
 *    versions of crAddFile() function. The crAddFile() macro defines character set
 *    independent mapping.
 *
 *  \sa crAddFileW(), crAddFileA(), crAddFile()
 */

CRASHRPTAPI(int)
crAddFileW(
   LPCWSTR pszFile,
   LPCWSTR pszDesc 
   );

/*! \ingroup DeprecatedAPI
 *  \copydoc crAddFileW()
 */


CRASHRPTAPI(int)
crAddFileA(
   LPCSTR pszFile,
   LPCSTR pszDesc 
   );

/*! \brief Character set-independent mapping of crAddFileW() and crAddFileA() functions. 
 *  \ingroup DeprecatedAPI
 */
#ifdef UNICODE
#define crAddFile crAddFileW
#else
#define crAddFile crAddFileA
#endif //UNICODE

// Flags for crAddFile2() function.

#define CR_AF_TAKE_ORIGINAL_FILE  0 //!< Take the original file (do not copy it to the error report folder).
#define CR_AF_MAKE_FILE_COPY      1 //!< Copy the file to the error report folder.

#define CR_AF_FILE_MUST_EXIST     0 //!< Function will fail if file doesn't exist at the moment of function call.
#define CR_AF_MISSING_FILE_OK     2 //!< Do not fail if file is missing (assume it will be created later).

/*! \ingroup CrashRptAPI  
 *  \brief Adds a file to crash report.
 * 
 *  \return This function returns zero if succeeded.
 *
 *  \param[in] pszFile Absolute path to the file to add.
 *  \param[in] pszDestFile Destination file name.
 *  \param[in] pszDesc File description (used in Error Report Details dialog).
 *  \param[in] dwFlags Flags.
 *
 *    This function can be called anytime after crInstall() to add one or more
 *    files to the generated crash report. 
 *  
 *    When this function is called, the file is marked to be added to the error report, 
 *    then the function returns control to the caller.
 *    When crash occurs, all marked files are added to the report. 
 *
 *    \a pszFile should be a valid absolute path of a file to add to crash report. It
 *    is recommended to add small files (several KB in size). If a large file is added,
 *    the crash report sending procedure may fail.
 *
 *    \a pszDestFile should be the name of destination file. This parameter can be used
 *    to specify different file name for the file in ZIP archive. If this parameter is NULL, the pszFile
 *    file name is used as destination file name.
 *
 *    \a pszDesc is a literal description of a file. It can be NULL.
 *
 *    \a dwFlags parameter defines the behavior of the function. This can be a combination of the following flags:
 *       - \ref CR_AF_TAKE_ORIGINAL_FILE  On crash, the CrashSender.exe will try to locate the file from its original location. This behavior is the default one.
 *       - \ref CR_AF_MAKE_FILE_COPY      On crash, the CrashSender.exe will make a copy of the file and save it to the error report folder.  
 *
 *       - \ref CR_AF_FILE_MUST_EXIST     The function will fail if file doesn't exist at the moment of function call (the default behavior). 
 *       - \ref CR_AF_MISSING_FILE_OK     Do not fail if file is missing (assume it will be created later).
 *
 *    If your file is not huge, specify the \ref CR_AF_MAKE_FILE_COPY as \a dwFlags parameter value. This will
 *    guarantee that a snapshot of your file at the moment of crash is taken and saved to the error report folder.
 *
 *    This function fails if \a pszFile doesn't exist at the moment of function call, unless you specify \ref CR_AF_MISSING_FILE_OK flag. 
 * 
 *    The crAddFile2W() and crAddFile2A() are wide-character and multibyte-character
 *    versions of crAddFile2() function. The crAddFile2() macro defines character set
 *    independent mapping.
 *
 *    This function is available <b>since v.1.2.1</b>. This function replaces the crAddFile() function.
 *
 *  \sa crAddFile2W(), crAddFile2A(), crAddFile2()
 */

CRASHRPTAPI(int)
crAddFile2W(
   LPCWSTR pszFile,
   LPCWSTR pszDestFile,
   LPCWSTR pszDesc,
   DWORD dwFlags
   );

/*! \ingroup CrashRptAPI
 *  \copydoc crAddFile2W()
 */

CRASHRPTAPI(int)
crAddFile2A(
   LPCSTR pszFile,
   LPCSTR pszDestFile,
   LPCSTR pszDesc,
   DWORD dwFlags
   );

/*! \brief Character set-independent mapping of crAddFile2W() and crAddFile2A() functions. 
 *  \ingroup CrashRptAPI
 */
#ifdef UNICODE
#define crAddFile2 crAddFile2W
#else
#define crAddFile2 crAddFile2A
#endif //UNICODE


// Flags for crAddScreenshot function.
#define CR_AS_VIRTUAL_SCREEN  0 //!< Take a screenshot of the virtual screen.
#define CR_AS_MAIN_WINDOW     1 //!< Take a screenshot of application main window.

/*! \ingroup CrashRptAPI  
 *  \brief Adds a screenshot to the crash report.
 * 
 *  \return This function returns zero if succeeded.
 *
 *  \param[in] dwFlags Flags.
 *  
 *  \remarks 
 *
 *  This function can be used to make a screenshot at the moment of crash and add
 *  the screenshot to the error report. Call this function inside of \ref LPGETLOGFILE crash callback.
 * 
 *  \a dwFlags can be one of the following:
 *    - \ref CR_AS_VIRTUAL_SCREEN Use this to take a screenshot of the desktop (virtual screen).
 *    - \ref CR_AS_MAIN_WINDOW    Use this to take a screenshot of the application's main window.
 *  
 *  Screenshots are added in form of PNG files. When capturing entire desktop consisting of several monitors, 
 *  one screenshot file is added per each monitor.
 *
 *  You should be carefull when using this feature, because screenshots may contain user-identifying 
 *  or private information. Always specify the purposes you will use the collected 
 *  information for in your Privacy Policy. Typically, the screenshot is not a required information to 
 *  debug the crash, however sometimes it may be useful.
 *
 *  \sa
 *   crAddFile2()
 */

CRASHRPTAPI(int)
crAddScreenshot(
   DWORD dwFlags
   );

/*! \ingroup CrashRptAPI  
 *  \brief Adds a string property to the crash report.
 * 
 *  \return This function returns zero if succeeded.
 *
 *  \param[in] pszPropName   Name of the property.
 *  \param[in] pszPropValue  Value of the property.
 *  
 *  \remarks 
 *
 *  Use this function to add a string property to the crash description XML file.
 *  User-added properties are listed under \<CustomProps\> tag of the XML file.
 *
 *  The following example shows how to add information about the amount of free disk space to the crash
 *  description XML file:
 *  \code
 *  // It is assumed that you already calculated the amount of free disk space, converted it to text
 *  // and store it as szFreeSpace string.
 *  LPCTSTR szFreeSpace = _T("0 Kb");
 *  crAddProperty(_T("FreeDiskSpace"), szFreeSpace);
 *
 *  \endcode
 *
 *  \sa
 *   crAddFile2(), crAddScreenshot()
 */

CRASHRPTAPI(int)
crAddPropertyW(
   LPCWSTR pszPropName,
   LPCWSTR pszPropValue
   );

/*! \ingroup CrashRptAPI
 *  \copydoc crAddPropertyW()
 */

CRASHRPTAPI(int)
crAddPropertyA(
   LPCSTR pszPropName,
   LPCSTR pszPropValue
   );

/*! \brief Character set-independent mapping of crAddPropertyW() and crAddPropertyA() functions. 
 *  \ingroup CrashRptAPI
 */
#ifdef UNICODE
#define crAddProperty crAddPropertyW
#else
#define crAddProperty crAddPropertyA
#endif //UNICODE

/*! \ingroup CrashRptAPI  
 *  \brief Adds a registry key dump to the crash report.
 * 
 *  \return This function returns zero if succeeded.
 *
 *  \param[in] pszRegKey        Registry key to dump, required.
 *  \param[in] pszDstFileName   Name of the destination file, required. 
 *  \param[in] dwFlags          Flags.
 *  
 *  \remarks 
 *
 *  Use this function to add a dump of a Windows registry key into the crash report. This function
 *  is available since v.1.2.6.
 *
 *  The \a pszRegKey parameter must be the name of the registry key. 
 *  The content of the key specified by the \a pszRegKey parameter will be stored in human-readable XML
 *  format and included into the error report as \a pszDstFileName file. 
 *
 *  The \a dwFlags parameter is reserved for future use and should be set to zero.
 *
 *  The following example shows how to dump two registry keys to regkey.xml file:
 *  \code
 *  
 *  crAddRegKey(_T("HKEY_CURRENT_USER\\Software\\MyApp"), _T("regkey.xml"));
 *  crAddRegKey(_T("HKEY_LOCAL_MACHINE\\Software\\MyApp"), _T("regkey.xml"));
 *
 *  \endcode
 *
 *  \sa
 *   crAddFile2(), crAddScreenshot(), crAddProperty()
 */

CRASHRPTAPI(int)
crAddRegKeyW(   
   LPCWSTR pszRegKey,
   LPCWSTR pszDstFileName,
   DWORD dwFlags
   );

/*! \ingroup CrashRptAPI
 *  \copydoc crAddRegKeyW()
 */

CRASHRPTAPI(int)
crAddRegKeyA(   
   LPCSTR pszRegKey,
   LPCSTR pszDstFileName,
   DWORD dwFlags
   );

/*! \brief Character set-independent mapping of crAddRegKeyW() and crAddRegKeyA() functions. 
 *  \ingroup CrashRptAPI
 */
#ifdef UNICODE
#define crAddRegKey crAddRegKeyW
#else
#define crAddRegKey crAddRegKeyA
#endif //UNICODE


// Exception types
#define CR_WIN32_STRUCTURED_EXCEPTION   0    //!< SEH exception (deprecated name, use \ref CR_SEH_EXCEPTION instead).
#define CR_SEH_EXCEPTION                0    //!< SEH exception.
#define CR_CPP_TERMINATE_CALL           1    //!< C++ terminate() call.
#define CR_CPP_UNEXPECTED_CALL          2    //!< C++ unexpected() call.
#define CR_CPP_PURE_CALL                3    //!< C++ pure virtual function call (VS .NET and later).
#define CR_CPP_NEW_OPERATOR_ERROR       4    //!< C++ new operator fault (VS .NET and later).
#define CR_CPP_SECURITY_ERROR           5    //!< Buffer overrun error (VS .NET only).
#define CR_CPP_INVALID_PARAMETER        6    //!< Invalid parameter exception (VS 2005 and later).
#define CR_CPP_SIGABRT                  7    //!< C++ SIGABRT signal (abort).
#define CR_CPP_SIGFPE                   8    //!< C++ SIGFPE signal (flotating point exception).
#define CR_CPP_SIGILL                   9    //!< C++ SIGILL signal (illegal instruction).
#define CR_CPP_SIGINT                   10   //!< C++ SIGINT signal (CTRL+C).
#define CR_CPP_SIGSEGV                  11   //!< C++ SIGSEGV signal (invalid storage access).
#define CR_CPP_SIGTERM                  12   //!< C++ SIGTERM signal (termination request).


/*! \ingroup CrashRptStructs
 *  \brief Extended exception info used by crGenerateErrorReport().
 *
 *  \remarks
 *
 *  This structure contains essential information needed to generate crash minidump file and
 *  provide the developer with other information about the error.
 *
 *  \a cb must contain the size of this structure in bytes.
 *
 *  \a pexcptrs should contain the exception pointers. If this parameter is NULL, 
 *     the current CPU state is used to generate exception pointers.
 *
 *  \a exctype is the type of exception. This value can be used for crash report classification on developers' side. 
 *  This parameter may be one of the following:
 *     - \ref CR_SEH_EXCEPTION             SEH (Structured Exception Handling) exception
 *     - \ref CR_CPP_TERMINATE_CALL        C++ terminate() function call
 *     - \ref CR_CPP_UNEXPECTED_CALL       C++ unexpected() function call
 *     - \ref CR_CPP_PURE_CALL Pure virtual method call (Visual Studio .NET 2003 and later) 
 *     - \ref CR_CPP_NEW_OPERATOR_ERROR C++ 'new' operator error (Visual Studio .NET 2003 and later)
 *     - \ref CR_CPP_SECURITY_ERROR Buffer overrun (Visual Studio .NET 2003 only) 
 *     - \ref CR_CPP_INVALID_PARAMETER Invalid parameter error (Visual Studio 2005 and later) 
 *     - \ref CR_CPP_SIGABRT C++ SIGABRT signal 
 *     - \ref CR_CPP_SIGFPE  C++ floating point exception
 *     - \ref CR_CPP_SIGILL  C++ illegal instruction
 *     - \ref CR_CPP_SIGINT  C++ SIGINT signal
 *     - \ref CR_CPP_SIGSEGV C++ invalid storage access
 *     - \ref CR_CPP_SIGTERM C++ termination request
 * 
 *   \a code is used if \a exctype is \ref CR_SEH_EXCEPTION and represents the SEH exception code. 
 *   If \a pexptrs is NULL, this value is used when generating exception information for initializing
 *   \c pexptrs->ExceptionRecord->ExceptionCode member, otherwise it is ignored.
 *
 *   \a fpe_subcode is used if \a exctype is equal to \ref CR_CPP_SIGFPE. It defines the floating point
 *   exception subcode (see \c signal() function ducumentation in MSDN).
 * 
 *   \a expression, \a function, \a file and \a line are used when \a exctype is \ref CR_CPP_INVALID_PARAMETER.
 *   These members are typically non-zero when using debug version of CRT.
 *
 *   <b>Since v.1.2.4</b>, \a bManual parameter should be equal to TRUE if the report is generated manually. 
 *   The value of \a bManual parameter affects the automatic application restart behavior. If the application
 *   restart is requested by the \ref CR_INST_APP_RESTART flag of CR_INSTALL_INFO::dwFlags structure member, and if \a bManual is FALSE, the application will be
 *   restarted after error report generation. If \a bManual is TRUE, the application won't be restarted.
 */

typedef struct tagCR_EXCEPTION_INFO
{
  WORD cb;                   //!< Size of this structure in bytes; should be initialized before using.
  PEXCEPTION_POINTERS pexcptrs; //!< Exception pointers.
  int exctype;               //!< Exception type.
  DWORD code;                //!< Code of SEH exception.
  unsigned int fpe_subcode;  //!< Floating point exception subcode.
  const wchar_t* expression; //!< Assertion expression.
  const wchar_t* function;   //!< Function in which assertion happened.
  const wchar_t* file;       //!< File in which assertion happened.
  unsigned int line;         //!< Line number.
  BOOL bManual;              //!< Flag telling if the error report is generated manually or not.
}
CR_EXCEPTION_INFO;

typedef CR_EXCEPTION_INFO *PCR_EXCEPTION_INFO;

/*! \ingroup CrashRptAPI  
 *  \brief Manually generates an errror report.
 *
 *  \return This function returns zero if succeeded. When failed, it returns a non-zero value.
 *     Use crGetLastErrorMsg() to retrieve the error message.
 *  
 *  \param[in] pExceptionInfo Exception information. 
 *
 *  \remarks
 *
 *    Call this function to manually generate a crash report. When crash information is collected,
 *    control is returned to the caller. The crGenerateErrorReport() doesn't terminate the caller process.
 *
 *    The crash report contains crash minidump, crash description in XML format and
 *    additional custom files added with crAddFile2().
 *
 *    The exception information should be passed using \ref CR_EXCEPTION_INFO structure. 
 *
 *    The following example shows how to use crGenerateErrorReport() function.
 *
 *    \code
 *    CR_EXCEPTION_INFO ei;
 *    memset(&ei, 0, sizeof(CR_EXCEPTION_INFO));
 *    ei.cb = sizeof(CR_EXCEPTION_INFO);
 *    ei.exctype = CR_SEH_EXCEPTION;
 *    ei.code = 1234;
 *    ei.pexcptrs = NULL;
 *
 *    int result = crGenerateErrorReport(&ei);
 *
 *    if(result!=0)
 *    {
 *      // If goes here, crGenerateErrorReport() has failed
 *      // Get the last error message
 *      TCHAR szErrorMsg[256];
 *      crGetLastErrorMsg(szErrorMsg, 256);
 *    }
 *   
 *    // Manually terminate program
 *    ExitProcess(0);
 *
 *    \endcode
 */

CRASHRPTAPI(int)
crGenerateErrorReport(   
   __in_opt CR_EXCEPTION_INFO* pExceptionInfo
   );


/*! \ingroup CrashRptAPI 
 *  \brief Can be used as a SEH exception filter.
 *
 *  \return This function returns \c EXCEPTION_EXECUTE_HANDLER if succeeds, else \c EXCEPTION_CONTINUE_SEARCH.
 *
 *  \param[in] code Exception code.
 *  \param[in] ep   Exception pointers.
 *
 *  \remarks
 *     
 *     This function can be called instead of a SEH exception filter
 *     inside of __try{}__except(Expression){} statement. The function generates a error report
 *     and returns control to the exception handler block.
 *
 *     The exception code is usually retrieved with \b GetExceptionCode() intrinsic function
 *     and the exception pointers are retrieved with \b GetExceptionInformation() intrinsic 
 *     function.
 *
 *     If an error occurs, this function returns \c EXCEPTION_CONTINUE_SEARCH.
 *     Use crGetLastErrorMsg() to retrieve the error message on fail.
 *
 *     The following example shows how to use crExceptionFilter().
 *    
 *     \code
 *     int* p = NULL;   // pointer to NULL
 *     __try
 *     {
 *        *p = 13; // causes an access violation exception;
 *     }
 *     __except(crExceptionFilter(GetExceptionCode(), GetExceptionInformation()))
 *     {   
 *       // Terminate program
 *       ExitProcess(1);
 *     }
 *
 *     \endcode 
 */

CRASHRPTAPI(int)
crExceptionFilter(
  unsigned int code, 
  __in_opt struct _EXCEPTION_POINTERS* ep);

#define CR_NONCONTINUABLE_EXCEPTION  32  //!< Non continuable sofware exception. 
#define CR_THROW                     33  //!< Throw C++ typed exception.

/*! \ingroup CrashRptAPI  
 *  \brief Emulates a predefined crash situation.
 *
 *  \return This function doesn't return if succeded. If failed, returns non-zero value. Call crGetLastErrorMsg()
 *   to get the last error message.
 *
 *  \param[in] ExceptionType Type of crash.
 *
 *  \remarks
 *
 *    This function uses some a priori incorrect or vulnerable code or raises a C++ signal or raises an uncontinuable
 *    software exception to cause crash.
 *
 *    This function can be used to test if CrashRpt handles a crash situation correctly.
 *    
 *    CrashRpt will intercept an error or exception if crInstall() and/or crInstallToCurrentThread2() 
 *    were previously called. crInstall() installs exception handlers that function on per-process basis.
 *    crInstallToCurrentThread2() installs exception handlers that function on per-thread basis.
 *    
 *  \a ExceptionType can be one of the following constants:
 *    - \ref CR_SEH_EXCEPTION  This will generate a null pointer exception.
 *    - \ref CR_CPP_TERMINATE_CALL This results in call of terminate() C++ function.
 *    - \ref CR_CPP_UNEXPECTED_CALL This results in call of unexpected() C++ function.
 *    - \ref CR_CPP_PURE_CALL This emulates a call of pure virtual method call of a C++ class instance (Visual Studio .NET 2003 and later).
 *    - \ref CR_CPP_NEW_OPERATOR_ERROR This emulates C++ new operator failure (Visual Studio .NET 2003 and later).
 *    - \ref CR_CPP_SECURITY_ERROR This emulates copy of large amount of data to a small buffer (Visual Studio .NET 2003 only).
 *    - \ref CR_CPP_INVALID_PARAMETER This emulates an invalid parameter C++ exception (Visual Studio 2005 and later). 
 *    - \ref CR_CPP_SIGABRT This raises SIGABRT signal (abnormal program termination).
 *    - \ref CR_CPP_SIGFPE This causes floating point exception.
 *    - \ref CR_CPP_SIGILL This raises SIGILL signal (illegal instruction signal).
 *    - \ref CR_CPP_SIGINT This raises SIGINT signal.
 *    - \ref CR_CPP_SIGSEGV This raises SIGSEGV signal.
 *    - \ref CR_CPP_SIGTERM This raises SIGTERM signal (program termination request).
 *    - \ref CR_NONCONTINUABLE_EXCEPTION This raises a noncontinuable software exception (expected result 
 *         is the same as in \ref CR_SEH_EXCEPTION).
 *    - \ref CR_THROW This throws a C++ typed exception (expected result is the same as in \ref CR_CPP_TERMINATE_CALL).
 *
 *  The \ref CR_SEH_EXCEPTION uses null pointer write operation to cause the access violation.
 *
 *  The \ref CR_NONCONTINUABLE_EXCEPTION has the same effect as \ref CR_SEH_EXCEPTION, but it uses
 *  \b RaiseException() WinAPI function to raise noncontinuable software exception.
 *
 *  The following example shows how to use crEmulateCrash() function.
 *
 *  \code
 *  // emulate null pointer exception (access violation)
 *  crEmulateCrash(CR_SEH_EXCEPTION);
 *  \endcode
 *
 */

CRASHRPTAPI(int)
crEmulateCrash(
  unsigned ExceptionType) throw (...);



/*! \ingroup CrashRptAPI 
 *  \brief Gets the last CrashRpt error message.
 *
 *  \return This function returns length of error message in characters. If output buffer is invalid, returns a negative number.
 *
 *  \param[out] pszBuffer Pointer to the buffer.
 *  \param[in]  uBuffSize Size of buffer in characters.
 *
 *  \remarks
 *
 *    This function gets the last CrashRpt error message. You can use this function
 *    to retrieve the text status of the last called CrashRpt function.
 *
 *    If buffer is too small for the error message, the message is truncated.
 *
 *  crGetLastErrorMsgW() and crGetLastErrorMsgA() are wide-character and multi-byte character versions
 *  of crGetLastErrorMsg(). The crGetLastErrorMsg() macro defines character set independent mapping.
 *
 *  The following example shows how to use crGetLastErrorMsg() function.
 *
 *  \code
 *  
 *  // .. call some CrashRpt function
 *
 *  // Get the status message
 *  TCHAR szErrorMsg[256];
 *  crGetLastErrorMsg(szErrorMsg, 256);
 *  \endcode
 *
 *  \sa crGetLastErrorMsgA(), crGetLastErrorMsgW(), crGetLastErrorMsg()
 */

CRASHRPTAPI(int)
crGetLastErrorMsgW(
  __out_ecount_z(uBuffSize) LPWSTR pszBuffer, 
  UINT uBuffSize);

/*! \ingroup CrashRptAPI
 *  \copydoc crGetLastErrorMsgW()
 *
 */

CRASHRPTAPI(int)
crGetLastErrorMsgA(
  __out_ecount_z(uBuffSize) LPSTR pszBuffer, 
  UINT uBuffSize);

/*! \brief Defines character set-independent mapping for crGetLastErrorMsgW() and crGetLastErrorMsgA().
 *  \ingroup CrashRptAPI
 */

#ifdef UNICODE
#define crGetLastErrorMsg crGetLastErrorMsgW
#else
#define crGetLastErrorMsg crGetLastErrorMsgA
#endif //UNICODE


//// Helper wrapper classes

#ifndef _CRASHRPT_NO_WRAPPERS

/*! \class CrAutoInstallHelper
 *  \ingroup CrashRptWrappers
 *  \brief Installs exception handlers in constructor and uninstalls in destructor.
 *  \remarks
 *    Use this class to easily install/uninstall exception handlers in you \b main()
 *    or \b WinMain() function.
 *
 *    This wrapper class calls crInstall() in its constructor and calls crUninstall() in
 *    its destructor.
 *
 *    Use CrAutoInstallHelper::m_nInstallStatus member to check the return status of crInstall().
 *   
 *    Example:
 *
 *    \code
 *    #include <CrashRpt.h>
 *
 *    void main()
 *    {      
 *      CR_INSTALL_INFO info;
 *      memset(&info, 0, sizeof(CR_INSTALL_INFO));
 *      info.cb = sizeof(CR_INSTALL_INFO);  
 *      info.pszAppName = _T("My App Name");
 *      info.pszAppVersion = _T("1.2.3");
 *      info.pszEmailSubject = "Error Report from My App v.1.2.3";
 *      // The address to send reports by E-mail
 *      info.pszEmailTo = _T("myname@hotmail.com");  
 *      // The URL to send reports via HTTP connection
 *      info.pszUrl = _T("http://myappname.com/utils/crashrpt.php"); 
 *      info.pfnCrashCallback = CrashCallback; 
 *      info.uPriorities[CR_HTTP] = 3; // Try HTTP first
 *      info.uPriorities[CR_SMTP] = 2; // Try SMTP second
 *      info.uPriorities[CR_SMAPI] = 1; // Try system email program last
 *
 *      // Install crash reporting
 *      CrAutoInstallHelper cr_install_helper(&info);
 *      // Check that installed OK
 *      assert(cr_install_helper.m_nInstallStatus==0);
 *
 *      // Your code follows here ...
 *
 *    }
 *    \endcode
 */

class CrAutoInstallHelper
{
public:

  //! Installs exception handlers to the caller process
  CrAutoInstallHelper(PCR_INSTALL_INFOA pInfo)
  {
    m_nInstallStatus = crInstallA(pInfo);
  }

  //! Installs exception handlers to the caller process
  CrAutoInstallHelper(PCR_INSTALL_INFOW pInfo)
  {
    m_nInstallStatus = crInstallW(pInfo);
  }

  //! Uninstalls exception handlers from the caller process
  ~CrAutoInstallHelper()
  {
    crUninstall();
  }

  //! Install status
  int m_nInstallStatus;
};

/*! \class CrThreadAutoInstallHelper
 *  \ingroup CrashRptWrappers
 *  \brief Installs (uninstalls) exception handlers for the caller thread in class' constructor (destructor).
 *  
 *  \remarks
 *
 *   This wrapper class calls crInstallToCurrentThread2() in its constructor and 
 *   calls crUninstallFromCurrentThread() in its destructor.
 *
 *   Use CrThreadAutoInstallHelper::m_nInstallStatus member to check 
 *   the return status of crInstallToCurrentThread2().
 *
 *   Example:
 *
 *   \code
 *   DWORD WINAPI ThreadProc(LPVOID lpParam)
 *   {
 *     CrThreadAutoInstallHelper cr_thread_install_helper();
 *     assert(cr_thread_install_helper.m_nInstallStatus==0);
 *    
 *     // Your code follows here ...
 *   }
 *   \endcode
 */

class CrThreadAutoInstallHelper
{
public:

  //! Installs exception handlers to the caller thread
  CrThreadAutoInstallHelper(DWORD dwFlags=0)
  {
    m_nInstallStatus = crInstallToCurrentThread2(dwFlags);    
  }

  //! Uninstalls exception handlers from the caller thread
  ~CrThreadAutoInstallHelper()
  {
    crUninstallFromCurrentThread();
  }

  //! Install status
  int m_nInstallStatus;
};

#endif //!_CRASHRPT_NO_WRAPPERS

#endif //_CRASHRPT_H_


