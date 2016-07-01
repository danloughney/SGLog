//
//  PublicHeader.h
//  SGLog
//
//  Created by Daniel Loughney on 8/24/11.
//  Copyright (c) 2011-2016 Spooky Group LLC. All rights reserved.
//

#ifndef __SGLog_h__
#define __SGLog_h__

@import UIKit;
@import Foundation;

// these defines control the compilation of the entire logging solution

// controls SGWARN. Warnings can be included in your final build
#ifndef DEV_WARN
#define DEV_WARN (1)
#endif


#ifdef DEBUG
// these control various SGLOG macros. Only available for use in DEBUG builds

#ifndef DEV_LOG
#define DEV_LOG (1)
#endif 

#ifndef DEV_MEMORY_WARNING
#define DEV_MEMORY_WARNING (1)
#endif 

#ifndef DEV_TIMER
#define DEV_TIMER (1)
#endif

#endif

#define LOGGING_OFF         (0)
#define LOGGING_BASIC       (1)
#define LOGGING_HIGH        (2)
#define LOGGING_NETWORK     (5)
#define LOGGING_DETAILED    (10)

#define SGLOG_CATEGORY_NONE (0)
#define SGLOG_PROTOCOL      (1 << 0)
#define SGLOG_CONNECTION    (1 << 1)
#define SGLOG_LOGFN         (1 << 2)
#define SGLOG_FILEDATA      (1 << 3)
#define SGLOG_LOADASSET     (1 << 4)
#define SGLOG_PREFETCH      (1 << 5)
#define SGLOG_RELEASE       (1 << 6)
#define SGLOG_MEMORYMAP     (1 << 7)
#define SGLOG_ALT_FLOW      (1 << 8)
#define SGLOG_LAYOUTSV      (1 << 9)
#define SGLOG_MARKCENTER    (1 << 10)
#define SGLOG_HIRESREQ      (1 << 11)
#define SGLOG_PLAYER        (1 << 12)
#define SGLOG_DATEBAR       (1 << 13)
#define SGLOG_DELEGATE      (1 << 14)
#define SGLOG_UPDATETIMERS  (1 << 15)
#define SGLOG_MATH          (1 << 16)
#define SGLOG_ACCESSIBILITY (1 << 17)
#define SGLOG_COLLECTIONVIEW (1 << 18)

#define SGLOG_ALL           (0xffffffff)
#define SGLOG_CATEGORY_ALL  (0xffffffff)

#define MAX_LOGGING LOGGING_DETAILED
#define MIN_LOGGING LOGGING_OFF

#define WEDEBUG_MAX_DATA_LEN    (128)

@interface SGLogger : NSObject
@property (readwrite, assign) NSUInteger logLevel;
@property (readwrite, assign) NSUInteger category;

+ (SGLogger*)defaultLogger;

+ (BOOL)basicLogging;
+ (BOOL)networkLogging;
+ (BOOL)detailedLogging;
+ (BOOL)categoryBasedLogging:(NSUInteger)category;
+ (void)setCategory:(NSUInteger)category;
+ (void)setLevel:(NSUInteger)level;

+ (void) incrementLevel;
+ (void) decrementLevel;

@end

// return Megabytes for the given integer
NSString* mbFormat(NSUInteger);

// return MB/KB for the given integer
NSString* kbFormat(long long);
NSString* kbFormat2(long long);

CG_INLINE NSArray* callStack();

CG_INLINE NSArray* callStack()
{
    NSArray *tmp = [NSThread callStackSymbols];
    return @[tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]];
    // return @[tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], tmp[6], tmp[7], tmp[8]];
    
}

CG_INLINE void viewHeirarchy(UIView*theView, const char*functionName, const char*parentDepthSetThisToDoubleQuotes);

CG_INLINE void viewHeirarchy(UIView*theView, const char*functionName, const char*parentDepthSetThisToDoubleQuotes)
{
    char depth[256];
    strcpy(depth, parentDepthSetThisToDoubleQuotes);
    strcat(depth, ">");
    
    NSLog(@"%s %s view(%@) parent(0x%08x)", functionName, depth, theView, (int)theView.superview);
    
    for (UIView *sv in theView.subviews) {
        viewHeirarchy(sv, functionName, depth);
    }
}


#if DEV_TIMER == 1
#define TIMELOG_START NSDate *__dttls__ __unused = [NSDate date];
#define DEVTIMESTAMP [NSString stringWithFormat:@" dt(%@)", __dttls__]
#define TIMELOG_END   { NSLog(@"%s --- %g ---", __PRETTY_FUNCTION__, [[NSDate date] timeIntervalSinceDate:__dttls__]); }
#else
#define TIMELOG_START
#define DEVTIMESTAMP @""
#define TIMELOG_END
#endif

// define DEVELOPMENT to get the SG_LOG... macros to resolve to code.  Change this to DEVELOPMENT_DONE when ready for release to production
#if DEV_LOG == 1
#define __SGLOG_MESSAGE(format, func) [NSString stringWithFormat:@"%s %@", func, arg]
#define SGLOG_DECLARE(arg) arg
#define SGLOGFN NSDate*__dt__ __unused = [NSDate date];{ if ([WeDebug detailedLogging] && [WeDebug categoryBasedLogging:SGLOG_LOGFN]) NSLog(@"%s >>%@", __PRETTY_FUNCTION__, DEVTIMESTAMP); }
#define SGLOGFN1(arg1) NSDate*__dt__ __unused = [NSDate date];{ if ([WeDebug detailedLogging] && [WeDebug categoryBasedLogging:SGLOG_LOGFN]) NSLog(@"%s >> arg(%@)%@", __PRETTY_FUNCTION__, arg1, DEVTIMESTAMP); }

#define SGLOGFEND { if ([WeDebug basicLogging] && [WeDebug categoryBasedLogging:SGLOG_LOGFN]) NSLog(@"%s << --- %g ---", __PRETTY_FUNCTION__, [[NSDate date] timeIntervalSinceDate:__dt__]); }

#define SGLOG(format) { if ([WeDebug basicLogging]) NSLog(__SGLOG_MESSAGE(format, __PRETTY_FUNCTION__)); }

#define SGLOG1(format, arg1) { if ([WeDebug basicLogging]) NSLog(__SGLOG_MESSAGE(format, __PRETTY_FUNCTION__), arg1); }

#define SGLOG2(format, arg1, arg2) { if ([WeDebug basicLogging]) NSLog(__SGLOG_MESSAGE(format, __PRETTY_FUNCTION__), arg1, arg2); }

#define SGLOG3(format, arg1, arg2, arg3) { if ([WeDebug basicLogging]) NSLog(__SGLOG_MESSAGE(format, __PRETTY_FUNCTION__), arg1, arg2, arg3); }

#define SGLOG4(format, arg1, arg2, arg3, arg4) { if ([WeDebug basicLogging]) NSLog(__SGLOG_MESSAGE(format, __PRETTY_FUNCTION__), arg1, arg2, arg3, arg4); }

#define SGLOG5(format, arg1, arg2, arg3, arg4, arg5) { if ([WeDebug basicLogging]) NSLog(__SGLOG_MESSAGE(format, __PRETTY_FUNCTION__), arg1, arg2, arg3, arg4, arg5); }

#define SGLOG6(format, arg1, arg2, arg3, arg4, arg5) { if ([WeDebug basicLogging]) NSLog(__SGLOG_MESSAGE(format, __PRETTY_FUNCTION__), arg1, arg2, arg3, arg4, arg5, arg6); }

#define SGLOGCATFN(category) NSDate*__dt__ __unused = [NSDate date];{ if ([WeDebug categoryBasedLogging:category] && [WeDebug basicLogging]) NSLog(@"%s >>%@", __PRETTY_FUNCTION__, DEVTIMESTAMP); }

#define SGLOGCATFN1(category, arg1) NSDate*__dt__ __unused = [NSDate date];{ if ([WeDebug categoryBasedLogging:category] && [WeDebug basicLogging]) NSLog(@"%s >> arg(%@)%@", __PRETTY_FUNCTION__, arg1, DEVTIMESTAMP); }

#define SGLOGCATFEND(category) { if ([WeDebug categoryBasedLogging:category] && [WeDebug basicLogging]) NSLog(@"%s << --- %g ---", __PRETTY_FUNCTION__, [[NSDate date] timeIntervalSinceDate:__dt__]); }

#define SGLOGCAT(category, format) { if ([WeDebug categoryBasedLogging:category]) NSLog(format, __PRETTY_FUNCTION__); }

#define SGLOGCAT1(category, format, arg1) { if ([WeDebug categoryBasedLogging:category]) NSLog(format, __PRETTY_FUNCTION__, arg1); }

#define SGLOGCAT2(category, format, arg1, arg2) { if ([WeDebug categoryBasedLogging:category]) NSLog(format, __PRETTY_FUNCTION__, arg1, arg2); }

#define SGLOGCAT3(category, format, arg1, arg2, arg3) { if ([WeDebug categoryBasedLogging:category]) NSLog(format, __PRETTY_FUNCTION__, arg1, arg2, arg3); }

#define SGLOGCAT4(category, format, arg1, arg2, arg3, arg4) { if ([WeDebug categoryBasedLogging:category]) NSLog(format, __PRETTY_FUNCTION__, arg1, arg2, arg3, arg4); }

#define SGLOGCAT5(category, format, arg1, arg2, arg3, arg4, arg5) { if ([WeDebug categoryBasedLogging:category]) NSLog(format, __PRETTY_FUNCTION__, arg1, arg2, arg3, arg4, arg5); }

#define SGLOGVIEWH(aView) viewHeirarchy(aView, __PRETTY_FUNCTION__, "");NSLog(@"\n")
#else

#define SGLOG_DECLARE(arg)
#define SGLOGFN
#define SGLOGFN1(arg1)
#define SGLOGFEND
#define SGLOG(format)
#define SGLOG1(format, arg1)
#define SGLOG2(format, arg1, arg2)
#define SGLOG3(format, arg1, arg2, arg3)
#define SGLOG4(format, arg1, arg2, arg3, arg4)
#define SGLOG5(format, arg1, arg2, arg3, arg4, arg5)
#define SGLOG6(format, arg1, arg2, arg3, arg4, arg5, arg6)
#define SGLOGCATFN(category)
#define SGLOGCATFN1(category, arg1)
#define SGLOGCATFEND
#define SGLOGCAT(category, format)
#define SGLOGCAT1(category, format, arg1)
#define SGLOGCAT2(category, format, arg1, arg2)
#define SGLOGCAT3(category, format, arg1, arg2, arg3)
#define SGLOGCAT4(category, format, arg1, arg2, arg3, arg4)
#define SGLOGCAT5(category, format, arg1, arg2, arg3, arg4, arg5)
#define SGLOGVIEWH(aView)
#endif

#ifdef DEVWARN
//#define SGWARNFN NSDate*__dt__ = [NSDate date];{ if ([WeDebug detailedLogging]) NSLog(@"%s >> dt(%@)", __PRETTY_FUNCTION__, __dt__); }
//#define SGWARNFN1(arg1) NSDate*__dt__ = [NSDate date];{ NSLog(@"%s >> arg(%@) dt(%@)", __PRETTY_FUNCTION__, arg1, __dt__); }
#define SGWARNFN { if ([WeDebug detailedLogging]) NSLog(@"%s >>", __PRETTY_FUNCTION__); }
#define SGWARNFN1(arg1) { NSLog(@"%s >> arg(%@)", __PRETTY_FUNCTION__, arg1); }
#define SGWARNFN2(arg1, arg2) { NSLog(@"%s >> arg(%@) arg2(%@)", __PRETTY_FUNCTION__, arg1, arg2); }

#define SGWARNFEND { NSLog(@"%s << --- %g ---", __PRETTY_FUNCTION__, [[NSDate date] timeIntervalSinceDate:__dt__]); }

#define SGWARN(format) { NSLog(format, __PRETTY_FUNCTION__); }

#define SGWARN1(format, arg1) { NSLog(format, __PRETTY_FUNCTION__, arg1); }
#define SGWARN2(format, arg1, arg2) { NSLog(format, __PRETTY_FUNCTION__, arg1, arg2); }

#define SGWARN3(format, arg1, arg2, arg3) { NSLog(format, __PRETTY_FUNCTION__, arg1, arg2, arg3); }

#define SGWARN4(format, arg1, arg2, arg3, arg4) { NSLog(format, __PRETTY_FUNCTION__, arg1, arg2, arg3, arg4); }

#define SGWARN5(format, arg1, arg2, arg3, arg4, arg5) { NSLog(format, __PRETTY_FUNCTION__, arg1, arg2, arg3, arg4, arg5); }
#define SGWARN6(format, arg1, arg2, arg3, arg4, arg5, arg6) { NSLog(format, __PRETTY_FUNCTION__, arg1, arg2, arg3, arg4, arg5, arg6); }
#define SGWARN7(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7) { NSLog(format, __PRETTY_FUNCTION__, arg1, arg2, arg3, arg4, arg5, arg6, arg7); }
#define SGWARN_DECLARE(arg) arg
#define SGWARN_FRAME(name, aView) { UIView *__v__ = aView; SGWARN5(@"%s %@ frame(%g,%g %g,%g)", name, __v__.frame.origin.x, __v__.frame.origin.y, __v__.frame.size.width, __v__.frame.size.height); }
#define SGWARN_RECT(name,  aRect) { SGWARN5(@"%s %@ rect(%g,%g %g,%g)", name, aRect.origin.x, aRect.origin.y, aRect.size.width, aRect.size.height); }
#define SGWARN_POINT(name, aPoint) { SGWARN3(@"%s %@ point(%g,%g)", name, aPoint.x, aPoint.y); }
#define SGWARN_SIZE(name,  aSize) { SGWARN3(@"%s %@ size(%g,%g)", name, aSize.width, aSize.height); }
#define SGWARNVIEWH(aView) viewHeirarchy(aView, __PRETTY_FUNCTION__, "");NSLog(@"\n")
#define SGWARN_CMTIME(name, cmTime) { SGWARN5(@"%s %@ cmTime(%lld, %d, %d, %lld)", name, cmTime.value, cmTime.timescale, cmTime.flags, cmTime.epoch); }
#define SGWARN_TRANSFORM(name, caTransform) { SGWARN7(@"%s %@ caTransform(a=%g, b=%g, c=%g, d=%g, tx=%g, ty=%g)", name, caTransform.a, caTransform.b, caTransform.c, caTransform.d, caTransform.tx, caTransform.ty); }
#define SGWARN_EXCEPTION(e) { SGWARN1(@"%s exception(%@", e); }
#else
#define SGWARNFN
#define SGWARNFN1(arg1)
#define SGWARNFN2(arg1,arg2)
#define SGWARNFEND
#define SGWARN(format)
#define SGWARN1(format, arg1)
#define SGWARN2(format, arg1, arg2)
#define SGWARN3(format, arg1, arg2, arg3)
#define SGWARN4(format, arg1, arg2, arg3, arg4)
#define SGWARN5(format, arg1, arg2, arg3, arg4, arg5)
#define SGWARN6(format, arg1, arg2, arg3, arg4, arg5, arg6)
#define SGWARN7(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define SGWARN_DECLARE(arg)
#define SGWARN_FRAME(name, aView)
#define SGWARN_RECT(name,  aRect)
#define SGWARN_POINT(name, aPoint)
#define SGWARN_SIZE(name,  aSize)
#define SGWARNVIEWH(aView)
#define SGWARN_CMTIME(name, cmTime)
#define SGWARN_TRANSFORM(name, caTransform)
#define SGWARN_EXCEPTION(e)
#endif

#ifdef DEV_MEMORY_WARNING
#define SGLOG_MEMORYWARNING NSLog(@"%s received memory warning", __PRETTY_FUNCTION__)
#else
#define SGLOG_MEMORYWARNING
#endif

void visWindows (UIView* view);

@interface SGVisWindows : NSObject
@property (readwrite, strong) NSMutableArray *views;
@property (readwrite, strong) UIView* currentView;
@property (readwrite, strong) UIColor* currentBackgroundColor;
@property (readwrite, assign) NSInteger currentIndex;
- (id) initWithView:(UIView*)rootView;
- (void) next;
@end


#endif /* PublicHeader_h */
