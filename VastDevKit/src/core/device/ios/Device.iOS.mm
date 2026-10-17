#include "Device.h"

#import <UIKit/UIKit.h>
#import "QuartzCore/QuartzCore.h"

static hd::Device::DeviceOrientation g_orientation = hd::Device::DEVICE_ORIENTATION_LANDSCAPE;
static float g_pixelCoordRatio = 1.0f;


void hd::Device::SetDeviceOrientation(hd::Device::DeviceOrientation orientation)
{
    g_orientation = orientation;
}


hd::Device::DeviceOrientation hd::Device::GetDeviceOrientation()
{
    return g_orientation;
}



void hd::Device::SetPixelCoordRatio(const float pixelCoordRatio)
{
    g_pixelCoordRatio = pixelCoordRatio;
}


float hd::Device::GetPixelCoordRatio()
{
    return g_pixelCoordRatio;
}


float hd::Device::GetContentScaleFactor()
{
    float scale = 1.0f;
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
    {
        if([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) 
        {
            scale = [[UIScreen mainScreen] scale];
        }
    }
    else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        scale = 1.0f;
    }
    
    return scale;
}


void hd::Device::SetDisplayScreenSize(const double w, const double h)
{
    // No need to do anything on iOS
}


float hd::Device::GetDisplayScreenWidth()
{
    CGRect bounds = [[UIScreen mainScreen] bounds];
    if (hd::Device::GetDeviceOrientation() == hd::Device::DEVICE_ORIENTATION_LANDSCAPE)
    {
        return MAX(bounds.size.width, bounds.size.height);
    }
    else
    {
        return MIN(bounds.size.width, bounds.size.height);
    }
}


float hd::Device::GetDisplayScreenHeight()
{
    CGRect bounds = [[UIScreen mainScreen] bounds];
    if (hd::Device::GetDeviceOrientation() == hd::Device::DEVICE_ORIENTATION_LANDSCAPE)
    {
        return MIN(bounds.size.width, bounds.size.height);
    }
    else
    {
        return MAX(bounds.size.width, bounds.size.height);
    }
}


float hd::Device::GetAspectRatio()
{
    hdAssert(hd::Device::GetDisplayScreenHeight() != 0.0f);
    return hd::Device::GetDisplayScreenWidth() / hd::Device::GetDisplayScreenHeight();
}


const char * hd::Device::GetUDID()
{
    if ([UIDevice currentDevice] &&
        [[UIDevice currentDevice] respondsToSelector:@selector(uniqueIdentifier)])
    {
        return [[[UIDevice currentDevice] uniqueIdentifier] UTF8String];
    }
    return "???";
}


double hd::Device::GetMediaTime()
{
    return (double)CACurrentMediaTime();
}



extern hd::Device::DeviceIdiom hd::Device::GetDeviceIdiom()
{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
    {
        return hd::Device::DEVICE_IDIOM_PHONE;
    }
    else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        return hd::Device::DEVICE_IDIOM_TABLET;
    }
    return hd::Device::DEVICE_IDIOM_UNDEFINED;
}
