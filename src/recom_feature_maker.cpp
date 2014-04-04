/*
 * 这个程序从默认的 <user, item, action_score, time, info1, info2, info3, ... > 中提取出 Factor Model 使用
 * 的 <label [feature_id:feature_value] feature_id:feature_value ... > 的 lib-
 * svm 格式的训练文件
 */

#include <iostream>

#include "recom_feature_maker.h"
#include "util.h"

void exit_with_help()
{
    printf("%s\n", "Usage: ./recom_feature_maker src_data_file des_data_file feature_signal");
    printf("%s\n", "    feature_signal = fea1 | fea2 | fea3 | ...");
    printf("%s\n", "        fea1 = (1 << 1) : user_id");
    printf("%s\n", "        fea2 = (1 << 2) : item_id");
    printf("%s\n", "        fea3 = (1 << 3) : other items related");
    printf("%s\n", "        fea4 = (1 << 4) : weekday");
    printf("%s\n", "        fea5 = (1 << 5) : hour");
    printf("%s\n", "        fea6 = (1 << 6) : day in month");
    printf("%s\n", "        fea7 = (1 << 7) : last item related");
    printf("%s\n", "        fea8 = (1 << 8) : info1");
    printf("%s\n", "        fea9 = (1 << 9) : info2");
}

int main(int argc, char ** argv)
{
    if (argc < 4)
    {
        exit_with_help();
        return -1;
    }

    std::string src_data_filepath = argv[1];
    std::string des_data_filepath = argv[2];
    std::string feature_signal = argv[3]; 
    // 构建 Feature List
    std::vector<Rhea::Feature *> feature_makers;
    uint32_t fea_sig_val = 0;
    sscanf(feature_signal.c_str(), "%u", &fea_sig_val);
    uint32_t fea_idx = 1;
    while (fea_sig_val != 0)
    {
        if (fea_sig_val & 0x0001)
        {
            switch(fea_idx)
            {
                case 1:
                    feature_makers.push_back(new Rhea::UserIDFeature());
                    break;
                case 2:
                    feature_makers.push_back(new Rhea::ItemIDFeature());
                    break;
                case 3:
                    feature_makers.push_back(new Rhea::ItemIDFFeature("test"));
                    break;
                case 4:
                    feature_makers.push_back(new Rhea::WeekdayFeature());
                    break;
                case 5:
                    feature_makers.push_back(new Rhea::HourFeature());
                    break;
                case 6:
                    feature_makers.push_back(new Rhea::DayFeature());
                    break;
                default:
                    std::cout << "Unknown feature signal" << std::endl;
                    break;
            }
        }
        ++ fea_idx;
        fea_sig_val = fea_sig_val >> 1;
    }

    // 每一行构建一个 RecomInfo 对象
    std::ifstream src_data_in(src_data_filepath.c_str());
    if (!src_data_in.is_open())
    {
        std::cout << "open src_data_file [" << src_data_filepath << "]" << std::endl;
        return -1;
    }

    std::ofstream des_data_out(des_data_filepath.c_str());
    if (!des_data_out.is_open())
    {
        std::cout << "open des_data_file [" << des_data_filepath << "]" << std::endl;
        src_data_in.close();
        return -1;
    }

    Rhea::RecomInfo recom_info;
    std::string src_data_line, token;
    std::vector< Rhea::SparseAtom<double> > features;
    while ( getline(src_data_in, src_data_line) )
    {
        recom_info.infos.clear();
        features.clear();
        std::stringstream stringsplitter(src_data_line);

        getline(stringsplitter, recom_info.user_id, '\t');
        getline(stringsplitter, recom_info.item_id, '\t');
        getline(stringsplitter, recom_info.action_score, '\t');
        getline(stringsplitter, recom_info.time, '\t');

        while (getline(stringsplitter, token, '\t'))
        {
            recom_info.infos.push_back(token);
        }

        // 对Feature List中的每一个Feature 调用 make_featuer 
        for (uint32_t i = 0; i < feature_makers.size(); ++i )
        {
            feature_makers[i]->make_feature(recom_info, features);
        }

        // 输出
        des_data_out << recom_info.action_score;
        for (uint32_t i = 0; i < features.size(); ++i )
        {
            des_data_out << " " << features[i].idx << 
                ":" << features[i].value;
        }
        des_data_out << std::endl;
    }

    des_data_out.close();
    src_data_in.close();

    for (uint32_t i = 0; i < feature_makers.size(); ++i )
    {
        DEL_POINTER(feature_makers[i]);
    }
    return 0;
}
