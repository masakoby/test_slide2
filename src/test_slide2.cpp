#include <iostream>
#include "kb_openslide.h"
#include "kb_xml_io.h"
#include "kb_xml_ndpa.h"

int main(int argc, char* argv[])
{
	std::string path_input = argv[1];
	std::string path_output = argv[2];
	int level = atoi(argv[3]);

	std::cout << path_input << std::endl;
	std::cout << path_output << std::endl;
	std::cout << level << std::endl;

	kb::COpenslide slide1;
	slide1.create(path_input);
	cv::Mat mat1;
	double ratio = 1.0;
	slide1.getImage_whole(mat1, level, ratio);

	slide1.m_info.print();


	std::string path_ndpa = path_input + ".ndpa";
	std::cout << path_ndpa << std::endl;

	std::vector<kb::slide_annotation> v_ndpa;
	kb::load_ndpa(path_ndpa, v_ndpa, slide1.m_info);

	int num_ndpa = v_ndpa.size();
	for (int k = 0; k < num_ndpa; k++) {
		//std::cout << v_ndpa[k].m_title << std::endl;
		int num = v_ndpa[k].m_pointlist_i.size();

		std::cout << k << ": " << 
			v_ndpa[k].m_annotation_type << " " << 
			num << " " << 
			v_ndpa[k].m_annotation_radius << 
			std::endl;

	}


	//std::string title1;
	//kb::read_xml(path_ndpa, "ndpviewstate", "title", title1);
	//std::cout << title1 << std::endl;
	//return 0;

	//if (0) {

	//	kb::COpenslide slide(path_input);

	//	cv::Mat mat1;
	//	double ratio = 1.0;
	//	slide.getImage_whole(mat1, level, ratio);
	//	cv::imwrite(path_output, mat1);

	//	cv::Size sz1 = mat1.size();
	//	std::cout << sz1 << std::endl;

	//	//{
	//	//	file << vfname1[k]
	//	//		<< "," << slide.m_info.m_vendor
	//	//		<< "," << slide.m_info.m_mag
	//	//		<< "," << slide.m_info.m_level_count
	//	//		<< "," << slide.m_info.m_sz[0]
	//	//		<< "," << slide.m_info.m_sz[1]
	//	//		<< "," << slide.m_info.m_mpp[0]
	//	//		<< "," << slide.m_info.m_mpp[1]
	//	//		<< "," << slide.m_info.m_offset[0]
	//	//		<< "," << slide.m_info.m_offset[1]
	//	//		<< "," << ratio
	//	//		<< std::endl;
	//	//}
	//}


	return 0;
}

