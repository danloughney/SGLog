//
//  SGLogger.m
//  SGLog
//
//  Created by Daniel Loughney on 8/24/11.
//  Copyright (c) 2011-2016 Spooky Group LLC. All rights reserved.
//
#import "PublicHeader.h"

static SGLogger *theLogger = nil;

@implementation SGLogger

+ (SGLogger*)defaultLogger {
    if (theLogger == nil)
        theLogger = [[SGLogger alloc]init];
    return theLogger;
}

+ (void)setLevel:(NSUInteger)level {
    [SGLogger defaultLogger].logLevel = level;
}
+ (void)setCategory:(NSUInteger)category {
    [SGLogger defaultLogger].category = category;
}

+ (BOOL)basicLogging {
    return ([SGLogger defaultLogger].logLevel >= LOGGING_BASIC);
}

+ (BOOL)networkLogging {
    return ([SGLogger defaultLogger].logLevel >= LOGGING_NETWORK);
}

+ (BOOL)detailedLogging {
    return ([SGLogger defaultLogger].logLevel >= LOGGING_DETAILED);
}

+ (BOOL)categoryBasedLogging:(NSUInteger)category {
    return (category & [SGLogger defaultLogger].category ? YES : NO);
}

- (NSString*) description {
    NSString *logDescription;
    switch (_logLevel) {
        case LOGGING_OFF:
            logDescription = @"Off";
            break;
            
        case LOGGING_BASIC:
            logDescription = @"Basic";
            break;
            
        case LOGGING_NETWORK:
            logDescription = @"Network";
            break;
            
        case LOGGING_DETAILED:
            logDescription = @"Detailed";
            break;
            
        default:
            logDescription = @"Unknown";
            break;
    }
    return [[NSString alloc]initWithFormat:@"<WeDebug 0x%08x logLevel(%@) category(0x%08lx)", (int)self, logDescription, (unsigned long)_category];
}

+ (void) incrementLevel {
    SGLogger *logger = [SGLogger defaultLogger];
    
    NSInteger _loggingLevel = logger.logLevel ++;
    if (_loggingLevel >= MAX_LOGGING)
        _loggingLevel = MAX_LOGGING;
    
    [logger setLogLevel:_loggingLevel];
}

+ (void) decrementLevel {
    SGLogger *logger = [SGLogger defaultLogger];
    
    NSInteger _loggingLevel = logger.logLevel --;
    if (_loggingLevel <= MIN_LOGGING)
        _loggingLevel = MIN_LOGGING;
    
    [logger setLogLevel:_loggingLevel];
}

@end


// creates a flat list of views from the view heirarchy
void walkViewHierarchy(UIView*v, NSMutableArray* array) {
    [array addObject:v];
    for (UIView *s in v.subviews)
        walkViewHierarchy(s, array);
}
void makeInvisibleViews(NSArray *array) {
    for (UIView*v in array) {
        NSNumber *alpha = @0.0f;
        [v performSelectorOnMainThread:@selector(setAlpha:) withObject:alpha waitUntilDone:YES];
    }
}
void makeVisibleViews(NSArray *array) {
    for (UIView*v in array) {
        NSNumber *alpha = @1.0f;
        [v performSelectorOnMainThread:@selector(setAlpha:) withObject:alpha waitUntilDone:YES];
    }
}
//void visHighlight(UIView*v) {
//    v.backgroundColor = [UIColor redColor];
//    v.alpha = 1;
//}

@implementation SGVisWindows

- (id) initWithView:(UIView*)rootView {
    self = [super init];
    if (self) {
        _views = [[NSMutableArray alloc]init];
        [self walkViewHierarchy:rootView withArray:_views];
        _currentIndex = 0;
        _currentView = _views[_currentIndex];
        _currentBackgroundColor = _currentView.backgroundColor;
        
        NSLog(@"%s views(%@)", __PRETTY_FUNCTION__, _views);
        for (UIView*v in _views) {
            //NSNumber *alpha = [NSNumber numberWithFloat:0.0];
            v.alpha = 0.3;
        }
    }
    return self;
}

- (void) walkViewHierarchy:(UIView*)rootView withArray:(NSMutableArray*)views {
    [views addObject:rootView];
    for (UIView *s in rootView.subviews)
        [self walkViewHierarchy:s withArray:views];
}

- (void) next {
    
    _currentView.backgroundColor = _currentBackgroundColor;
    _currentView.alpha = 0.3;
    
    _currentIndex++;
    if (_currentIndex == [_views count]) {
        SGWARN1(@"%s REACHED END OF VIEW HIERARCHY at view #%ld.  Starting over at 0.", (long)_currentIndex);
        _currentIndex = 0;
    }
    _currentView = _views[_currentIndex];
    _currentBackgroundColor = _currentView.backgroundColor;
    _currentView.backgroundColor = [UIColor purpleColor];
    NSLog(@"%s currentView(%@)", __PRETTY_FUNCTION__, _currentView);
    
    _currentView.alpha = 1.0;
}

@end


void visWindows (UIView* view) {
    // this method walks the entire tree of subviews and alernately makes them visible/hidden
    NSLog(@"%s", __PRETTY_FUNCTION__);
    
    NSMutableArray*array = [[NSMutableArray alloc]init];
    walkViewHierarchy(view, array);
    
    makeInvisibleViews(array);
    
    UITextView *text = [[UITextView alloc]initWithFrame:CGRectMake(0, 0, 300, 200)];
    [view addSubview:text];
    
    //        [UIView animateWithDuration:10.0 animations:^{
    for (UIView *v in array) {
        UIColor *saveColor = v.backgroundColor;
        NSNumber *alphaLevelOn = @1.0f;
        [v performSelectorOnMainThread:@selector(setAlpha:) withObject:alphaLevelOn waitUntilDone:YES];
        [v performSelectorOnMainThread:@selector(setBackgroundColor:) withObject:[UIColor redColor] waitUntilDone:YES];
        
        text.text = [v description];
        NSLog(@"%@", text.text);
        
        //            usleep(50000);
        
        NSNumber *alphaLevelOff = @0.0f;
        [v performSelectorOnMainThread:@selector(setAlpha:) withObject:alphaLevelOff waitUntilDone:YES];
        [v performSelectorOnMainThread:@selector(setBackgroundColor:) withObject:saveColor waitUntilDone:YES];
    }
    //        }];
    
    makeVisibleViews(array);
    [text removeFromSuperview];
}

NSString* mbFormat(NSUInteger value) {
    if (value < 1000)
        return [[NSString alloc]initWithFormat:@"%lu", (unsigned long)value];
    
    if (value < 100000)
        return [[NSString alloc]initWithFormat:@"%lu,%03lu", (unsigned long)value / 1000, (unsigned long)value % 1000];
    
    if (value < 1000000)
        return [[NSString alloc]initWithFormat:@"%lu,%03lu", (unsigned long)(value /1000)%1000, (unsigned long)value % 1000];
    
    return [[NSString alloc]initWithFormat:@"%lu.%lumb", (unsigned long)value / 1000000, (unsigned long)value % (1000000)/10000];
}

NSString* kbFormat(long long value) {
    // pass 1000, return 1KB
    // pass 1000000, return 100KB
    // pass 10000000 return 1MB
    
    if (value == 0) return nil;
    
    if (value < 1000)
        return [[NSString alloc]initWithFormat:@"%lu", (unsigned long)value];
    
    if (value < 1000000)
        return [[NSString alloc]initWithFormat:@"%luKB", (unsigned long)value / 1000];
    
    //    if (value < 1000000)
    //        return [[NSString alloc]initWithFormat:@"%luMB", (unsigned long)(value /1000)%1000];
    
    return [[NSString alloc]initWithFormat:@"%lu.%luMB", (unsigned long)value / 1000000, (unsigned long)value % (1000000)/10000];
}

NSString* kbFormat2(long long value) {
    // pass 1000, return 1KB
    // pass 1000000, return 100KB
    // pass 10000000 return 1MB
    
    if (value == 0)
        return @"Empty";
    
    if (value < 1000)
        return [[NSString alloc]initWithFormat:@"%lu", (unsigned long)value];
    
    if (value < 1000000)
        return [[NSString alloc]initWithFormat:@"%luKB", (unsigned long)value / 1000];
    
    //    if (value < 1000000)
    //        return [[NSString alloc]initWithFormat:@"%luMB", (unsigned long)(value /1000)%1000];
    
    return [[NSString alloc]initWithFormat:@"%lu.%luMB", (unsigned long)value / 1000000, (unsigned long)value % (1000000)/100000];
}
