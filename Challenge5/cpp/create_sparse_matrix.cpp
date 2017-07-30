#include <iostream>
#include <ios>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <limits>

#include <map>
#include <string>
#include <utility>

#include <exception>
#include <stdexcept>

#include <cstdlib>
#include <cerrno>
#include <cstring>

void progress(double progress, unsigned int width=50, char label='#') {
	unsigned int ii; 
	unsigned int count = progress * width;

	std::cerr << "\r[";
	for(ii=0; ii < count; ii++) {
		std::cerr << label;
	}   
	for(; ii < width; ii++) {
		std::cerr << ' ';
	}   
	std::cerr << "] ";

	std::cerr << std::setfill(' ') << std::setw(5) << std::fixed << std::setprecision(3) << (100*progress) << "%";
	std::cerr.flush();
}

void usage(const char *file_name) {
	std::cout << "edges.csv to Sparse Matrix" << std::endl << std::endl;

	std::cout << "Usage:  " << file_name << " [--matrix-file] [--mapping-file] infile" << std::endl;
	std::cout << "  --matrix-file   location to write matrix file, standard out by default" << std::endl;
	std::cout << "  --mapping-file  location to write mapping file" << std::endl;
}

bool parse_args(int argc, char *argv[], std::string &matrix_file, std::string &mapping_file, std::string &infile) {
	matrix_file.clear();
	mapping_file.clear();
	infile.clear();

	int ii=1;
	while(ii < argc) {
		if((strcmp(argv[ii], "-h") == 0) || (strcmp(argv[ii], "--help") == 0)) {
			usage(argv[0]);

			return false;
		} 
		else if(strcmp(argv[ii], "--matrix-file") == 0) {
			ii++;
			if(ii >= argc) {
				std::cerr << "Option " << argv[ii-1] << " requires an argument." << std::endl << std::endl;
				usage(argv[0]);

				return false;
			}

			matrix_file.assign(argv[ii]);
		}
		else if(strcmp(argv[ii], "--mapping-file") == 0) {
			ii++;
			if(ii >= argc) {
				std::cerr << "Option " << argv[ii-1] << " requires an argument." << std::endl << std::endl;
				usage(argv[0]);

				return false;
			}

			mapping_file.assign(argv[ii]);
		}
		else if(infile.empty()) {
			infile.assign(argv[ii]);
		}
		else {
			std::cerr << "Unrecognized input option " << argv[ii] << std::endl << std::endl;
			usage(argv[0]);

			return false;
		}  

		ii++;
	}

	if(infile.empty()) {
		std::cerr << "Missing input file" << std::endl;
		usage(argv[0]);

		return false;
	}

	return true;
}

void write_sparse_matrix(const std::string &infile, std::ostream &output, std::map<std::string,unsigned int> &mapping) {	
	std::ifstream file(infile.c_str());

	if(file) {
		std::string line, src_str, dst_str;
		std::istringstream iss;
		std::streampos total, current;
		std::pair<std::map<std::string,unsigned int>::iterator, bool> res;

		file.seekg(0, std::ios_base::end);
		total = file.tellg();
		file.clear();
		file.seekg(0, std::ios_base::beg);

		current = file.tellg();
		unsigned int line_num = 0;
		while(getline(file, line)) {
			current = file.tellg();
			if(line_num % 100000 == 0 || current == total) {
				progress(current/(double)total);
			}
			
			line_num++;

			iss.clear();
			iss.str(line);

			if(!getline(iss, src_str, ';')) {
				std::ostringstream oss;
				oss << std::endl << infile << ":" << line_num << ": missing source" << std::endl;

				throw std::runtime_error(oss.str());
			}
			if(!getline(iss, dst_str)) {
				std::ostringstream oss;
				oss << std::endl << infile << ":" << line_num << ": missing destination" << std::endl;

				throw std::runtime_error(oss.str());
			}
		
			res = mapping.insert(std::make_pair(src_str, (unsigned int)(mapping.size()+1)));
			output << res.first->second << " ";

			res = mapping.insert(std::make_pair(dst_str, (unsigned int)(mapping.size()+1)));
			output << res.first->second << " 1" << std::endl;
		}

		output << mapping.size() << " " << mapping.size() << " 0" << std::endl;

		std::cerr << std::endl;

		if(file.bad()) {
			std::ostringstream oss;
			oss << infile << ": " << strerror(errno);
			
			throw std::runtime_error(oss.str());
		}
	}
	else {
		std::ostringstream oss;
		oss << infile << ": " << strerror(errno);

		throw std::runtime_error(oss.str());
	}
}

void write_mapping(std::ostream &output, const std::map<std::string,unsigned int> &mapping) {
	std::map<std::string,unsigned int>::const_iterator mapping_iter = mapping.begin();
	for(; mapping_iter != mapping.end(); ++mapping_iter) {
		output << mapping->first << "," << mapping->second << std::endl;
	}
}

int create_sparse_matrix(const std::string &infile, const std::string &matrix_file, const std::string &mapping_file) {

	std::map<std::string,unsigned int> mapping;
	if(matrix_file.empty()) {
		write_sparse_matrix(infile, std::cout, mapping);
	}
	else {
		std::ofstream file(matrix_file.c_str());
		if(file) {
			write_sparse_matrix(matrix_file.c_str, file, mapping);
			if(!file) {
				std::ostringstream oss;
				oss << matrix_file.c_str << ": " << strerror(errno);

				throw std::runtime_error(oss.str());
			}
		}
		else {
			std::ostringstream oss;
			oss << matrix_file.c_str << ": " << strerror(errno);

			throw std::runtime_error(oss.str());
		}
	}

	if(!mapping_file.empty()) {
		
	}

	return 0;
}

int main(int argc, char *argv[]) {
	std::string matrix_file, mapping_file, infile;

	if(parse_args(argc, argv, matrix_file, mapping_file, infile)) {
		try {
			create_sparse_matrix(infile, matrix_file, mapping_file);
		}
		catch(std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
	}
}
