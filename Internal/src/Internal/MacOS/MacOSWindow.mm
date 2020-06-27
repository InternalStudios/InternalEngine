#import "MacOSWindow.h"
#import "Cocoa/Cocoa.h"
#import "Internal/Vulkan/VulkanContext.h"

namespace Internal
{
	MacOSWindow::MacOSWindow(const WindowData& data)
		: Window(data)
	{
		NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

		[NSApplication sharedApplication];

		NSUInteger windowStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;

		NSRect windowRect = NSMakeRect(100, 100, data.width, data.height);
		NSWindow* window = [[NSWindow alloc] initWithContentRect:windowRect styleMask:windowStyle backing:NSBackingStoreBuffered defer:NO];
		[window autorelease];

		NSWindowController* windowController = [[NSWindowController alloc] initWithWindow:window];
		[windowController autorelease];

		@autoreleasepool
		{
			NSString* string = @(data.Title);
			[window setTitle:string];

			[window setMiniwindowTitle:string];
		}

		[window orderFrontRegardless];

		[pool drain];
		m_Context.Init();

	}

	MacOSWindow::~MacOSWindow()
	{
		m_Context.Shutdown();
	}
}