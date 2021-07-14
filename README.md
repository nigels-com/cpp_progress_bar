C++ Progress Bar
================

C++ class that implements a progress bar in console applications on both Linux and Windows platforms. The progress bar works by referencing an index that keeps updating when processing through a piece of code.

This fork is a single-threaded header-only C++17 derivative based on previous work.

![Demo](doc/demo.gif)

Other known flavours:

  * [Mikhail Karasikov](https://github.com/karasikov/cpp_progress_bar)
  * [Hemant Tailor](https://github.com/htailor/cpp_progress_bar)

We made progress, so should you!

Key features
-------------

1. Limit the progress redrawing based on a millisecond timeout.
2. The ability to adjust the style of the progress bar.
3. The length of the progress bar displayed on screen adapts to the the width of the console.

Implementation
===========

To use the progress bar, include the following header file:

```C++
#include "progress_bar.h"
```

There is no need to build or link a library.

Creating a progress bar
------------------------

To create a progress bar, the total number of jobs need to be known.

```C++   
int n = 10;
Progress::Bar progress_bar;
progress_bar.setTotal(n);
```

Optionally, small descriptions can also be added to the progress bar.

```C++ 
int n = 10;
Progress::Bar progress_bar;
progress_bar.setTotal(n).setDescription("Example 1");
```


Updating a progress bar
-------------------------

Updates to the progress bar are made using the increment operators.


**Example 1:**

```C++
int n = 10;
Progress::Bar progress_bar;
progress_bar.setTotal(n).setDescription("Example 1");

int job_index = 0;

do_some_work();

job_index++;
progress_bar += 1;

do_some_more_work();

job_index++;
progress_bar += 1;
```

**Example 2:**

```C++
int n = 100;
Progress::Bar progress_bar;
progress_bar.setTotal(n).setDescription("Example 1");

for (int i = 0; i <= n; ++i) {
    ++progress_bar;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
```

Setting the frequency of updates
----------------------------------

Using the method `setTimeout(size_t timeout)` limits the frequency of redrawing.

**Example 3:**
```C++
int n = 100000;
Progress::Bar progress_bar;
progress_bar.setTotal(n).setDescription("Example 1").setTimeout(50); // 20Hz

for (int i = 0; i <= n; ++i){
    ++progress_bar;
}
```

Changing the bar style
------------------------

The progress bar can be customised using the `setStyle()` method. This changes the ASCII/Unicode characters used for printing the progress bar.

**Example 4**
```C++
n = 1000;

Progress::Style style;
style._empty = "-";
style._full = "\u2588";

int n = 1000;
Progress::Bar progress_bar;
progress_bar.setTotal(n).setDescription("Example 3").setStyle(style);

for (int i = 0; i <= n; ++i) {
    ++progress_bar;
}
```


Main Example
=========

```C++
#include "progress_bar.h"

#include <thread>

int main() {

    /// Example 1
    {
        int n = 90;
        Progress::Bar bar;
        bar.setDescription("Example #1").setTotal(n).setTimeout(50).setStyle(Progress::line_utf8);

        for (int i = 0; i < n; ++i) {
            ++bar;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    /// Example 2
    {
        int n = 1500;
        Progress::Bar bar;
        bar.setDescription("Example #2").setTotal(n).setTimeout(50).setStyle(Progress::filled_utf8);
        for (int i = 0; i < n; ++i) {
            ++bar;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }


    /// Example 3
    {
        Progress::Style style;
        style._empty = "-";
        style._full = "\u2588";

        int n = 100;
        Progress::Bar progress_bar;
        progress_bar.setTotal(n).setDescription("Example 3").setStyle(style);

        for (int i = 0; i <= n; ++i) {
            ++progress_bar;
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
        }
    }

    return EXIT_SUCCESS;
}
```
