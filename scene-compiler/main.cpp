#include <QCoreApplication>
#include <iostream>
#include "compiler.h"

void usage() {
	std::cout << "Usage: narcc"
		<< " [PARAMETER] JSON_FILE OUTPUT_FILE\n"
		<< "Parameters:\n"
		<< "-r\t\t compile resource package\n"
		<< "-s\t\t compile scene\n";
}

int main(int argc, char *argv[]) {
	int ret = 0;
	QCoreApplication a(argc, argv);
	auto arg_list = a.arguments();
	if (arg_list.size() != 4) {
		usage();
		ret = 1;
	}
	else if (arg_list[1] == "-r")
		compile_package(arg_list[2].toStdString(),
				arg_list[3].toStdString());
	else if (arg_list[1] == "-s")
		compile_scene(arg_list[2].toStdString(),
				arg_list[3].toStdString());
	else {
		usage();
		ret = 1;
	}
	return ret;
}
