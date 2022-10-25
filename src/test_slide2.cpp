#include <iostream>
#include "kb_openslide.h"

int main(int argc, char* argv[])
{
	std::string path_input = argv[1];
	std::string path_output = argv[2];
	int level = atoi(argv[3]);

	{
		kb::COpenslide slide(path_input);

		cv::Mat mat1;
		double ratio = 1.0;
		slide.getImage_whole(mat1, level, ratio);
		cv::imwrite(path_output, mat1);

		cv::Size sz1 = mat1.size();
		std::cout << sz1 << std::endl;

		//{
		//	file << vfname1[k]
		//		<< "," << slide.m_info.m_vendor
		//		<< "," << slide.m_info.m_mag
		//		<< "," << slide.m_info.m_level_count
		//		<< "," << slide.m_info.m_sz[0]
		//		<< "," << slide.m_info.m_sz[1]
		//		<< "," << slide.m_info.m_mpp[0]
		//		<< "," << slide.m_info.m_mpp[1]
		//		<< "," << slide.m_info.m_offset[0]
		//		<< "," << slide.m_info.m_offset[1]
		//		<< "," << ratio
		//		<< std::endl;
		//}
	}


	return 0;
}

