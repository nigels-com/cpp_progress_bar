#include "progress_bar.h"

#include <thread>

int main() {

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

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
		progress_bar.setTotal(n).setDescription("Example #3").setStyle(style);

		for (int i = 0; i < n; ++i) {
		    ++progress_bar;
			std::this_thread::sleep_for(std::chrono::milliseconds(40));
		}
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	return EXIT_SUCCESS;
}
