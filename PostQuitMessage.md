# 1.一种注意点

在非UI线程调用PostQuitMessage试图结束进程，结果没有任何效果。

原因：

PostQuitMessage只会为当前线程发送WM_QUIT消息，退出当前的UI线程。但调用这个函数的当前线程是一个的线程池中的线程（比如std::thread）时调用这个函数没有任何效果，不会让UI线程退出。

改动之前的逻辑是在UI线程调用此函数，所以是正确的。这个改动之后的逻辑是在线程池中调用此函数，是错误的。

解决：

在非UI线程想结束UI线程，可以通过PostMessage(hwnd, WM_QUIT, 0, 0)函数来实现。

这个hwnd参数是主线程（这个UI线程）的一个窗口句柄

# 2. 

 在Win32应用程序的主函数中有一段消息循环收发的代码:
 while (GetMessage(&Msg, hWnd, 0, 0))
 {
  TranslateMessage(&Msg);
  DispatchMessage(&Msg);
 }
 在Debug环境下启动程序后点击关闭按钮可以正常关闭掉窗口，但是VS2008却无法正常退出执行状态(即应用程序没有正常返回)。于是我按如下步骤进行了Debug：
 (1).在消息处理函数中WM_DESTORY的处理中设置断点，点击关闭按钮case WM_DESROTY代码段可以执行；
 (2).当程序执行到case WM_DESROTY代码段之后再在主函数的TranslateMessage(&Msg)处设置断点
 (3).按下F5发现程序可以执行到TranslateMessage(&Msg)处。
 经过上述步骤Debug后发现当在WM_DESTORY消息处理过程中调用PostQuitMessage(0)函数后程序的消息循环代码段可以继续执行，但是按MSDN的描述PostQuitMessage函数会往线程的消息队列中插入一个WM_QUIT消息，而GetMessage函数在收到WM_QUIT消息后会返回一个非正数。
 后来在网上搜索网友克塞前来拜访的一段解释：
 对PostQuitMessage而言，它实际上并不发送WM_QUIT消息，他所做的实际是设置一些特殊的系统标志，而队列存取体系根据这个标志并结合一些比较复杂的算法来决定在某时某处引发实际的WM_QUIT。当算法临界时，GetMessage(hwnd)照样能获取WM_QUIT。而如果你PostThreadMessage(WM_QUIT)，那么一个实际的WM_QUIT消息会被队列，GetMessage(hwnd)永远不会获得这个退出消息。一个最简单的满足算法要求的实验可以是这样：你在WM_CLOSE消息里PostQuitMessage，然后在启动程序后，首先移动窗口，然后关闭窗口，则程序能够正常退出，设置断点调试可以发现，GetMessage(hwnd)确实捕获了这个理论上发给线程队列的退出消息，并且打印内存栈可以发现，消息本身确实是发给线程队列而非窗口队列的。反之，如果你先使hwnd失效，则无论你如何操作，GetMessage(hwnd)永远不会收到退出消息。所以总结起来说，最终能否退出首先取决于hwnd是否失效，虽然这并不是能解释这个问题的根本原因，但至少是一个正确的部分表象原因。