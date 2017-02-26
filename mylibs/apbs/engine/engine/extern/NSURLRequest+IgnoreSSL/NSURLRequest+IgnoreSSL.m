#import "NSURLRequest+IgnoreSSL.h"

@implementation NSURLRequest (IgnoreSSL)

+ (BOOL)allowsAnyHTTPSCertificateForHost:(NSString*)host
{
    // ignore certificate errors only for this domain
	if ([host hasSuffix:@"elasticbeanstalk.com"] || [host hasSuffix:@"elb.amazonaws.com"] || [host hasSuffix:@"abot.sh"])
    {
        return YES;
    }
    else
    {
        return NO;
    }
}

@end