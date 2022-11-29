#include <iostream>
#include "kb_openslide.h"
#include "kb_xml_io.h"
#include "kb_xml_ndpa.h"
#include "kb_searchfiles.h"
#include "kb_split_string.h"
#include "kb_cv_string.h"
#include "kb_csv.h"
#include "kb_cv_arrange_cvmat.h"


int main(int argc, char* argv[])
{
	std::string dirname_input = argv[1];
	std::string dirname_output = argv[2];
	int level = atoi(argv[3]);

	std::cout << dirname_input << std::endl;
	std::cout << dirname_output << std::endl;
	std::cout << level << std::endl;

	std::vector<std::vector<std::string>> vv;
	std::map<std::string, int> vcase;
	{
		vcase["LSIL"] = 1;
		vcase["HSIL"] = 2;
		vcase["LAH"] = 3;
		vcase["ASCUS"] = 4;
		vcase["ASC-H"] = 5;
		vcase["ASCUS/ASC-H"] = 6;


		std::string path_case = dirname_input + "case_information.txt";
		kb::load_csv(path_case, vv, ',');

		//int num_vv = vv.size();
		//for (int i = 0; i < 3; i++) {
		//	int num_v = vv[i].size();
		//	for (int j = 0; j < num_v; j++) {
		//		std::cout << vv[i][j] << " ";
		//	}
		//	std::cout << std::endl;
		//}
		//return 0;
	}

	std::vector<std::string>  vfnames;
	if (kb::search_files(dirname_input, "*.ndpa", vfnames) < 0)
		return -1;

	std::string path_output_txt = dirname_output + "all_cells.csv";
	std::ofstream f1(path_output_txt);
	f1 << "ndpi,case,x,y,doctor_index,ai_index,doctor_diag,ai_diag,HSIL score, HSIL rank, LSIL score, LSIL rank" << std::endl;

	std::string path_output_txt2 = dirname_output + "ndpi_summary.csv";
	std::ofstream f2(path_output_txt2);
	f2 << "ndpi,case, cell number, detected number" << std::endl;

	kb::stringF font1;
	font1.m_fontScale = 6;
	font1.m_thickness = 20;

	std::vector<int> v_ok1, v_ok2, v_all;
	std::vector<cv::Mat> vmat_ok, vmat_ng1, vmat_ng2;
	std::vector<int> vflag_vmat_ok;
	int num_files = vfnames.size();

	v_all.resize(num_files, 0);
	v_ok1.resize(num_files, 0);
	v_ok2.resize(num_files, 0);
	for (int kk = 0; kk < num_files; kk++) {
		std::string path_ndpa = dirname_input + vfnames[kk];
		std::cout << "---- " << kk << " / " << num_files << " ----" << std::endl;
		std::cout << path_ndpa << std::endl;

		int len_fname = vfnames[kk].length();
		std::string fname_ndpi = vfnames[kk].substr(0, len_fname - 5);
		std::string path_input = dirname_input + fname_ndpi;
		std::string path_output = dirname_output + fname_ndpi + ".jpg";

		std::string case1;
		int idx_case1 = 0;
		int num_vv = vv.size();
		for (int i = 0; i < num_vv; i++) {
			if (vv[i][0] == fname_ndpi) {
				case1 = vv[i][1];

				std::map<std::string, int>::iterator it;
				it = vcase.find(case1);

				if (it != vcase.end()) {
					idx_case1 = it->second;
				}
			}
		}

		std::cout << case1 << " " << idx_case1 << std::endl;
		//continue;

		kb::COpenslide slide1;
		if (slide1.create(path_input) < 0)
			return -1;
		cv::Mat mat1;
		double ratio = 1.0;
		slide1.getImage_whole(mat1, level, ratio);

		slide1.m_info.print();



		std::vector<kb::slide_annotation> v_ndpa;
		if (kb::load_ndpa(path_ndpa, v_ndpa, slide1.m_info) < 0)
			return -1;

		std::vector<cv::Point2d> vp_doctor;
		std::vector<cv::Point2d> vp_ai;
		std::vector<int> v_doctor;
		std::vector<int> v_ai;
		std::vector<int> v_doctor_diag;
		std::vector<int> v_ai_diag;


		double rr = 0;
		int num_ndpa = v_ndpa.size();
		for (int k = 0; k < num_ndpa; k++) {
			//std::cout << v_ndpa[k].m_title << std::endl;
			int num = v_ndpa[k].m_pointlist_d.size();


			if (v_ndpa[k].m_title == "show area")
				continue;

			std::cout << k << ": " <<
				v_ndpa[k].m_annotation_type << " " <<
				num << " " <<
				v_ndpa[k].m_annotation_radius << " ";//

			//	doctor's annotation
			if (v_ndpa[k].m_annotation_type == "circle") {
				if (num == 2) {
					cv::Point2d p1(v_ndpa[k].m_pointlist_d[0], v_ndpa[k].m_pointlist_d[1]);
					vp_doctor.push_back(p1);
					v_doctor.push_back(k);


					if (v_ndpa[k].m_color[0] == 0 && v_ndpa[k].m_color[1] == 0 && v_ndpa[k].m_color[2] == 255) {
						v_doctor_diag.push_back(2);
					}
					else if (v_ndpa[k].m_color[0] == 0 && v_ndpa[k].m_color[1] == 255 && v_ndpa[k].m_color[2] == 255) {
						v_doctor_diag.push_back(1);
					}
					else {
						v_doctor_diag.push_back(0);
					}

					std::cout << "x,y="<<p1.x << " " << p1.y << " ";
				}
			}
			else {
				if (num == 8) {
					cv::Point2d p2(0, 0);
					for (int i = 0; i < 4; i++) {
						cv::Point2d p1(v_ndpa[k].m_pointlist_d[i * 2 + 0], v_ndpa[k].m_pointlist_d[i * 2 + 1]);
						p2 += p1;
					}
					p2.x = p2.x / 4.0;
					p2.y = p2.y / 4.0;
					vp_ai.push_back(p2);
					v_ai.push_back(k);

					if (v_ndpa[k].m_color[0] == 0 && v_ndpa[k].m_color[1] == 0 && v_ndpa[k].m_color[2] == 255) {
						v_ai_diag.push_back(3);
					}
					else if (v_ndpa[k].m_color[0] == 0 && v_ndpa[k].m_color[1] == 255 && v_ndpa[k].m_color[2] == 255) {
						v_ai_diag.push_back(2);
					}
					else if (v_ndpa[k].m_color[0] == 255 && v_ndpa[k].m_color[1] == 255 && v_ndpa[k].m_color[2] == 255) {
						v_ai_diag.push_back(0);
					}
					else {
						v_ai_diag.push_back(1);
					}


					std::cout << "x,y=" << p2.x << " " << p2.y << " ";

					if (rr <= 0) {
						double r = v_ndpa[k].m_pointlist_d[0] - v_ndpa[k].m_pointlist_d[2];
						if (r < 0.0) {
							r = -r;
						}
						rr = r;
						//rr = r * 2;
					}
				}
			}
			std::cout << "rr="<<rr << " ";
			std::cout << std::endl;
		}


		double threshold2 = rr * rr;
		int num_doctor = vp_doctor.size();
		int num_ai = vp_ai.size();

		std::vector<int> vflag_ok_ng;
		vflag_ok_ng.resize(num_doctor, -1);
		std::vector<int> v_ai_idx;
		v_ai_idx.resize(num_doctor, -1);

		for (int d = 0; d < num_doctor; d++) {

			int a_selected = -1;
			double d2_min = -1;
			for (int a = 0; a < num_ai; a++) {
				cv::Point2d dv = vp_doctor[d] - vp_ai[a];
				double d2 = dv.x * dv.x + dv.y * dv.y;
				//std::cout << vp_doctor[d].x << ", " << vp_doctor[d].y << ", " << vp_ai[a].x << ", " << vp_ai[a].y <<": "<<d2 << std::endl;
				//getchar();
				if (d2 < threshold2) {
					if (d2_min < 0 || d2 < d2_min) {
						d2_min = d2;
						a_selected = a;
					}
				}
			}
			if (a_selected >= 0) {
				vflag_ok_ng[d] = v_ai[a_selected];
				v_ai_idx[d] = a_selected;

				if (v_ai_diag[a_selected] > 0) {
					v_ok2[kk] += 1;
				}
				v_ok1[kk] += 1;
			}
		}

		v_all[kk] = num_doctor;
		f2 << fname_ndpi << "," << case1 << "," << v_all[kk] << "," << v_ok1[kk] << "," << v_ok2[kk] << std::endl;


		std::vector<int> v_doctor_tp;
		v_doctor_tp.resize(num_doctor, -1);
		for (int d = 0; d < num_doctor; d++) {
			f1 << fname_ndpi << "," << case1 << ",";
			f1 << vp_doctor[d].x << "," << vp_doctor[d].y << ",";

			f1 << v_doctor[d] << "," << vflag_ok_ng[d] << ",";

			{
				int k1 = v_ai_idx[d];
				if (v_ai_idx[d] >= 0) {
					f1 << v_doctor_diag[d] << "," << v_ai_diag[k1] << ",";
				}
				else {
					f1 << v_doctor_diag[d] << ",-1," ;
				}

			}
			


			if (vflag_ok_ng[d] >= 0) {
				int k1 = vflag_ok_ng[d];
				std::vector<std::string> vstr1;
				kb::split_string(v_ndpa[k1].m_details, '\n', vstr1);
				int num_str1 = vstr1.size();
				if (num_str1 >= 2) {
					//f1 << vstr1[0] << "," << vstr1[1] << ",";
					for (int ii = 0; ii < 2; ii++) {
						std::vector<std::string> vstr2;
						kb::split_string(vstr1[ii], ':', vstr2);
						int num_str2 = vstr2.size();
						if (num_str2 >= 2) {
							std::vector<std::string> vstr3;
							kb::split_string(vstr2[1], '(', vstr3);
							int num_str3 = vstr3.size();
							if (num_str3 >= 2) {
								f1 << vstr3[0] << ",";
								int len_str3 = vstr3[1].length();
								f1 << vstr3[1].substr(4, len_str3 - 5) << ",";
								v_doctor_tp[d] = 1;
							}
						}
					}

				}
			}
			f1 << std::endl;

		}

		for (int d = 0; d < num_doctor; d++) {
			cv::Point p1(vp_doctor[d].x * ratio, vp_doctor[d].y * ratio);

			int r1 = 15;
			if (vflag_ok_ng[d] >= 0) {
				if (v_doctor_tp[d] == 1) {
					cv::circle(mat1, p1, r1, cv::Scalar(0, 0, 255), 3);
					font1.m_color = cv::Scalar(0, 0, 255);
				}
				else {
					cv::circle(mat1, p1, r1, cv::Scalar(0, 128, 255), 3);
					font1.m_color = cv::Scalar(0, 128, 255);
				}
			}
			else {
				cv::circle(mat1, p1, r1, cv::Scalar(0, 255, 0), 3);
				font1.m_color = cv::Scalar(0, 255, 0);
			}
			char buf1[128];
			snprintf(buf1, 128, "%d", v_doctor[d]);
			std::string str_buf1 = buf1;
			font1.putText(mat1, str_buf1, p1);
		}
		cv::imwrite(path_output, mat1);


		for (int d = 0; d < num_doctor; d++) {
			cv::Mat mat1;
			int w = 200;
			slide1.getImage(mat1, vp_doctor[d].x-w/2, vp_doctor[d].y - w / 2, w, w, 0);

			if (vflag_ok_ng[d] >= 0) {
				if (v_doctor_tp[d] == 1) {
					vmat_ok.push_back(mat1);

					{
						int k1 = v_ai_idx[d];
						int xx = -1;
						if (v_ai_idx[d] >= 0) {
							if (1 <= idx_case1 && idx_case1 <= 3) {
								if (v_doctor_diag[d] == 2 && v_ai_diag[k1] == 3) {
									xx = 23;
								}
								else if (v_doctor_diag[d] == 2 && v_ai_diag[k1] == 2) {
									xx = 22;
								}
								else if (v_doctor_diag[d] == 1 && v_ai_diag[k1] == 3) {
									xx = 13;
								}
								else if (v_doctor_diag[d] == 1 && v_ai_diag[k1] == 2) {
									xx = 12;
								}
								else {
									xx = 1;
								}
							}
							else {
								xx = 0;
							}
						}
						vflag_vmat_ok.push_back(xx);
					}
				}
				else {
					vmat_ng1.push_back(mat1);
				}
			}
			else {
				vmat_ng2.push_back(mat1);
			}
		}

		{
			int num_ok = vmat_ok.size();
			int num_ng1 = vmat_ng1.size();
			int num_ng2 = vmat_ng2.size();
			std::cout << "num_ok=" << num_ok << std::endl;
			std::cout << "num_ng1=" << num_ng1 << std::endl;
			std::cout << "num_ng2=" << num_ng2 << std::endl;
			int step = 100;
			int num_width = 20;
			std::vector<cv::Mat> vmat_ok23, vmat_ok12, vmat_ok22, vmat_ok13, vmat_ok1, vmat_ok0, vmat_ok_else;
			for (int k = 0; k < num_ok; k ++) {
				if (vflag_vmat_ok[k] == 23) {
					vmat_ok23.push_back(vmat_ok[k]);
				}
				else if (vflag_vmat_ok[k] == 12) {
					vmat_ok12.push_back(vmat_ok[k]);
				}
				else if (vflag_vmat_ok[k] == 13) {
					vmat_ok13.push_back(vmat_ok[k]);
				}
				else if (vflag_vmat_ok[k] == 22) {
					vmat_ok22.push_back(vmat_ok[k]);
				}
				else if (vflag_vmat_ok[k] == 1) {
					vmat_ok1.push_back(vmat_ok[k]);
				}
				else if (vflag_vmat_ok[k] == 0) {
					vmat_ok0.push_back(vmat_ok[k]);
				}
				else{
					vmat_ok_else.push_back(vmat_ok[k]);
				}
			}
			std::cout << "num_ok23=" << vmat_ok23.size() << std::endl;
			std::cout << "num_ok12=" << vmat_ok12.size() << std::endl;
			std::cout << "num_ok22=" << vmat_ok22.size() << std::endl;
			std::cout << "num_ok13=" << vmat_ok13.size() << std::endl;
			std::cout << "num_ok1=" << vmat_ok1.size() << std::endl;
			std::cout << "num_ok0=" << vmat_ok0.size() << std::endl;
			std::cout << "num_ok_else=" << vmat_ok_else.size() << std::endl;

			{
				int num_xx = vmat_ok23.size();
				for (int k = 0; k < num_xx; k += step) {
					cv::Mat matV;
					kb::arrange_matN_subset(vmat_ok23, matV, num_width, k, step);
					char buf1[128];
					snprintf(buf1, 128, "ok23_%05d.jpg", k);
					std::string str_buf1 = buf1;
					std::string path1 = dirname_output + buf1;
					cv::imwrite(path1, matV);
				}
			}
			{
				int num_xx = vmat_ok12.size();
				for (int k = 0; k < num_xx; k += step) {
					cv::Mat matV;
					kb::arrange_matN_subset(vmat_ok12, matV, num_width, k, step);
					char buf1[128];
					snprintf(buf1, 128, "ok12_%05d.jpg", k);
					std::string str_buf1 = buf1;
					std::string path1 = dirname_output + buf1;
					cv::imwrite(path1, matV);
				}
			}
			{
				int num_xx = vmat_ok22.size();
				for (int k = 0; k < num_xx; k += step) {
					cv::Mat matV;
					kb::arrange_matN_subset(vmat_ok22, matV, num_width, k, step);
					char buf1[128];
					snprintf(buf1, 128, "ok22_%05d.jpg", k);
					std::string str_buf1 = buf1;
					std::string path1 = dirname_output + buf1;
					cv::imwrite(path1, matV);
				}
			}
			{
				int num_xx = vmat_ok13.size();
				for (int k = 0; k < num_xx; k += step) {
					cv::Mat matV;
					kb::arrange_matN_subset(vmat_ok13, matV, num_width, k, step);
					char buf1[128];
					snprintf(buf1, 128, "ok13_%05d.jpg", k);
					std::string str_buf1 = buf1;
					std::string path1 = dirname_output + buf1;
					cv::imwrite(path1, matV);
				}
			}
			{
				int num_xx = vmat_ok1.size();
				for (int k = 0; k < num_xx; k += step) {
					cv::Mat matV;
					kb::arrange_matN_subset(vmat_ok1, matV, num_width, k, step);
					char buf1[128];
					snprintf(buf1, 128, "ok1_%05d.jpg", k);
					std::string str_buf1 = buf1;
					std::string path1 = dirname_output + buf1;
					cv::imwrite(path1, matV);
				}
			}
			{
				int num_xx = vmat_ok0.size();
				for (int k = 0; k < num_xx; k += step) {
					cv::Mat matV;
					kb::arrange_matN_subset(vmat_ok0, matV, num_width, k, step);
					char buf1[128];
					snprintf(buf1, 128, "ok0_%05d.jpg", k);
					std::string str_buf1 = buf1;
					std::string path1 = dirname_output + buf1;
					cv::imwrite(path1, matV);
				}
			}
			{
				int num_xx = vmat_ok_else.size();
				for (int k = 0; k < num_xx; k += step) {
					cv::Mat matV;
					kb::arrange_matN_subset(vmat_ok_else, matV, num_width, k, step);
					char buf1[128];
					snprintf(buf1, 128, "ok_else_%05d.jpg", k);
					std::string str_buf1 = buf1;
					std::string path1 = dirname_output + buf1;
					cv::imwrite(path1, matV);
				}
			}

			for (int k = 0; k < num_ok; k += step) {
				cv::Mat matV;
				kb::arrange_matN_subset(vmat_ok, matV, num_width, k, step);

				char buf1[128];
				snprintf(buf1, 128, "ok_%05d.jpg", k);
				std::string str_buf1 = buf1;
				std::string path1 = dirname_output + buf1;
				cv::imwrite(path1, matV);
			}
			for (int k = 0; k < num_ng1; k += step) {
				cv::Mat matV;
				kb::arrange_matN_subset(vmat_ng1, matV, num_width, k, step);

				char buf1[128];
				snprintf(buf1, 128, "ng1_%05d.jpg", k);
				std::string str_buf1 = buf1;
				std::string path1 = dirname_output + buf1;
				cv::imwrite(path1, matV);
			}
			for (int k = 0; k < num_ng2; k += step) {
				cv::Mat matV;
				kb::arrange_matN_subset(vmat_ng2, matV, num_width, k, step);

				char buf1[128];
				snprintf(buf1, 128, "ng2_%05d.jpg", k);
				std::string str_buf1 = buf1;
				std::string path1 = dirname_output + buf1;
				cv::imwrite(path1, matV);
			}
		}
		//getchar();
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

