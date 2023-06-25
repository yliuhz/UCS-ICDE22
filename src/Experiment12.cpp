//
// Created by liuyue on 20/4/2021.
// TIPS: 测时间，只测email, k=3, \theta=0.8, N变化
// 1520
//

#include <bits/stdc++.h>
#include "../include/base.h"
#include "../include/locals.h"
#include "../include/probabilistic_kcore_computation.h"
#include "../include/mcs_with_prune.h"
#include "../include/ss-vl.h"
#include "../include/ss-sc.h"
#include "../include/ls.h"
#include "../include/lssa-vl.h"
#include "../include/lssa-sc.h"

using namespace std;

double norm2(vector<double> &v1, vector<double> &v2){
    double ret = 0.0;
    int n = v1.size();
    assert(v2.size() == n);
    for(int i = 0; i < n; i++){
        ret += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }
    return sqrt(ret);
}

int main(void)
{
    // Parameters
    int qn = 20;
    int n0 = 1; // 50
    double theta = 0.8;

    vector<string> files = {
            "email-Enron-p.txt",
            "flickr.txt",
            "dblp-graph-p-18-20.txt",
            "biomine.txt",
    };

    vector<vector<vector<int>>> qs = {
            { // email-Enron
                    { // k = 3
                            19, 21, 31, 35, 41, 52, 65, 124, 142, 159,
                            161, 162, 176, 179, 181, 182, 189, 204, 253, 302,
                    },
                    { // k = 5
                            16, 237, 270, 272, 321, 468, 571, 609, 648, 649,
                            701, 702, 722, 763, 911, 912, 916, 942, 993, 1023,
                    },
                    { // k = 10
                            1, 7, 13, 55, 317, 330, 348, 397, 447, 476,
                            503, 718, 731, 746, 765, 821, 1029, 1135, 1136, 1150,
                    },
                    { // k = 15
                            96, 297, 350, 389, 405, 413, 496, 629, 660, 695,
                            705, 755, 789, 1149, 1211, 1311, 1328, 1352, 1596, 1866,
                    },
                    { // k = 20
                            217, 226, 506, 529, 541, 665, 709, 800, 801, 959,
                            1089, 1129, 1175, 1290, 1331, 1364, 1429, 1483, 1484, 1485,
                    },
                    { // k = 25
                            178, 331, 379, 548, 654, 799, 814, 838, 898, 930,
                            933, 951, 1177, 1204, 1246, 1361, 1362, 1465, 1477, 1550,
                    },
                    { // k = 30
                            112, 246, 309, 377, 381, 518, 535, 536, 711, 730,
                            751, 758, 895, 937, 1034, 1128, 1173, 1238, 1289, 1302,
                    },
                    { // k = 35
                            82, 138, 197, 206, 307, 320, 500, 545, 547, 602,
                            656, 667, 704, 736, 766, 798, 822, 859, 876, 880,
                    },
                    { // k = 40
                            27, 73, 102, 173, 193, 261, 271, 280, 281, 301,
                            312, 344, 406, 438, 472, 559, 568, 603, 615, 620,
                    }
            },
            { // Flickr
                    { // k = 3
                            46, 127, 154, 206, 223, 226, 475, 569, 571, 691,
                            737, 748, 884, 891, 1048, 1088, 1149, 1168, 1171, 2648,
                    },
                    { // k = 5
                            169, 180, 182, 303, 531, 574, 662, 678, 735, 896,
                            953, 984, 1044, 1084, 1169, 3119, 3285, 3286, 3289, 3899,
                    },
                    { // k = 10
                            109, 120, 183, 346, 431, 434, 448, 455, 641, 647,
                            693, 719, 773, 981, 1017, 3017, 3029, 3080, 3204, 3681,
                    },
                    { // k = 15
                            119, 139, 813, 888, 1054, 1195, 1240, 1241, 2915, 2938,
                            3197, 3203, 3268, 3734, 3743, 3745, 3758, 3969, 3984, 4047,
                    },
                    { // k = 20
                            287, 883, 916, 974, 1166, 1193, 3208, 3283, 3284, 3666,
                            3668, 3777, 3864, 4037, 4546, 4681, 4861, 4869, 5084, 5207,
                    },
                    { // k = 25
                            549, 728, 805, 1076, 1175, 2598, 2881, 3033, 3081, 3085,
                            3237, 3426, 3678, 4021, 5025, 5042, 5512, 5622, 5628, 5716,
                    },
                    { // k = 30
                            54, 340, 435, 436, 582, 598, 601, 605, 607, 613,
                            623, 624, 628, 630, 631, 634, 635, 637, 639, 702,
                    },
                    { // k = 35
                            275, 473, 597, 599, 899, 1091, 1098, 1116, 2898, 2906,
                            2916, 3126, 3145, 3161, 3353, 3918, 3978, 4891, 5109, 5174,
                    },
                    { // k = 40
                            709, 780, 807, 922, 2893, 2903, 3092, 3155, 3218, 3721,
                            3931, 3993, 4008, 4082, 4116, 4449, 4468, 4642, 4968, 4978,
                    },
                    { // k = 45
                            208, 422, 467, 471, 615, 643, 674, 946, 1071, 1081,
                            1124, 2604, 2909, 3314, 3364, 3503, 3726, 3753, 4084, 4089,
                    },
                    { // k = 50
                            159, 370, 433, 481, 593, 663, 671, 972, 976, 1096,
                            1097, 1101, 1108, 1110, 1114, 2636, 2878, 3150, 3590, 4463,
                    },
                    { // k = 55
                            166, 417, 616, 707, 710, 2662, 2945, 3065, 3456, 3979,
                            4101, 4105, 4109, 4117, 4120, 4136, 4156, 4164, 4530, 4833,
                    },
                    { // k = 60
                            12, 268, 667, 767, 993, 1080, 1213, 2677, 3045, 3053,
                            3153, 3257, 3383, 3388, 3395, 3437, 3444, 3512, 3581, 3891,
                    },
                    { // k = 120
                            1043, 2781, 2793, 4277, 4417, 5126, 5385, 5987, 6022, 6082,
                            6461, 6465, 7399, 7462, 8929, 8951, 14407, 14434, 14440, 15517,
                    }
            },
            { // DBLP
                    { // k = 3
                            2, 49, 64, 65, 66, 73, 109, 140, 151, 154,
                            160, 162, 209, 210, 225, 246, 259, 294, 315, 332,
                    },
                    { // k = 5
                            60, 63, 92, 134, 155, 172, 215, 216, 239, 240,
                            242, 245, 247, 268, 272, 298, 308, 317, 329, 331,
                    },
                    { // k = 10
                            18, 33, 62, 76, 98, 107, 111, 128, 157, 171,
                            219, 222, 261, 273, 302, 311, 324, 325, 347, 350,
                    },
                    { // k = 15
                            52, 88, 100, 183, 220, 221, 440, 508, 527, 565,
                            682, 687, 754, 756, 758, 797, 826, 1244, 1287, 1328,
                    },
                    { // k = 20
                            31, 69, 186, 189, 235, 447, 501, 509, 536, 570,
                            577, 782, 804, 951, 952, 1107, 1127, 1399, 1746, 1777,
                    },
                    { // k = 25
                            5, 158, 241, 525, 541, 601, 623, 655, 658, 673,
                            793, 1273, 1327, 1330, 1332, 1333, 1615, 1693, 2259, 2380,
                    },
                    { // k = 30
                            660, 776, 1122, 1159, 1161, 1446, 2128, 3541, 3732, 4727,
                            4817, 4851, 5301, 5329, 5336, 5559, 6184, 6872, 7326, 7696,
                    },
                    { // k = 35
                            416, 2052, 2562, 2995, 3115, 3756, 3948, 4805, 5751, 5752,
                            5916, 6663, 6664, 6665, 6667, 7881, 7885, 8015, 9012, 10487,
                    },
                    { // k = 40
                            692, 886, 1063, 1065, 1066, 1906, 2258, 4246, 6223, 6369,
                            10382, 10561, 10594, 15651, 16201, 16269, 18185, 19370, 24773, 25477,
                    },
                    { // k = 45
                            135, 336, 4178, 9137, 10777, 18730, 18943, 20230, 20231, 25783,
                            27958, 28465, 29581, 29781, 31851, 31852, 32479, 36647, 37306, 37478,
                    },
                    { // k = 50
                            6495, 6496, 15916, 25040, 25041, 25303, 25607, 30138, 72591, 77815,
                            77816, 91593, 93262, 104629, 104651, 104652, 104653, 104654, 104655, 104656,
                    },
                    { // k = 55
                            338, 21002, 28435, 130921, 130923, 202401, 350927, 432932, 487781, 508611,
                            587342, 626764, 644118, 653557, 713753, 713756, 713816, 713859, 714825, 715132,
                    },
                    { // k = 60
                            14848, 24495, 73889, 80902, 94829, 114548, 237426, 350236, 350237, 365644,
                            424609, 424616, 424625, 469241, 469242, 469249, 469275, 570863, 603663, 675594,
                    },
            },
            { // Biomine
                    { // k = 3
                            26, 54, 55, 57, 91, 104, 112, 113, 125, 126,
                            127, 135, 136, 139, 140, 141, 143, 144, 145, 171,
                    },
                    { // k = 5
                            119, 124, 168, 180, 208, 224, 235, 270, 271, 325,
                            353, 356, 378, 432, 477, 478, 480, 533, 540, 577,
                    },
                    { // k = 10
                            43, 53, 60, 62, 170, 394, 414, 424, 434, 436,
                            502, 602, 637, 821, 896, 1035, 1070, 1118, 1185, 1188,
                    },
                    { // k = 15
                            83, 84, 162, 179, 228, 234, 406, 444, 576, 698,
                            738, 741, 790, 888, 918, 919, 954, 1004, 1032, 1193,
                    },
                    { // k = 20
                            15, 32, 36, 73, 74, 86, 274, 395, 405, 812,
                            956, 1182, 1258, 1318, 1430, 1446, 1462, 1482, 1499, 1637,
                    },
                    { // k = 25
                            20, 310, 456, 766, 877, 1161, 1214, 1361, 1404, 1416,
                            1437, 1559, 1619, 1691, 1781, 1978, 2005, 2136, 2150, 2151,
                    },
                    { // k = 30
                            87, 107, 187, 207, 298, 343, 437, 653, 773, 807,
                            1031, 1398, 1475, 1587, 1620, 1621, 1779, 1870, 1881, 2076,
                    },
                    { // k = 35
                            2, 22, 93, 430, 445, 610, 630, 672, 911, 914,
                            1123, 1375, 1561, 1721, 1889, 1898, 2072, 2140, 2498, 3051,
                    },
                    { // k = 40
                            109, 118, 218, 225, 387, 682, 1334, 1369, 1589, 1722,
                            1890, 2256, 2510, 2776, 2782, 2862, 3019, 3361, 3482, 3716,
                    },
                    { // k = 45
                            78, 351, 390, 573, 705, 768, 1769, 1854, 1856, 2046,
                            2209, 2285, 2298, 2308, 2317, 2373, 2500, 2735, 2784, 3312,
                    },
                    { // k = 50
                            231, 306, 334, 565, 687, 1549, 1834, 1926, 2413, 2472,
                            3360, 3681, 3728, 4448, 5821, 5856, 5863, 5987, 6005, 6167,
                    },
                    { // k = 55
                            301, 317, 1980, 2210, 2385, 2818, 3126, 3715, 3842, 3923,
                            4037, 7209, 7260, 7270, 7447, 7459, 7506, 7643, 7784, 7804,
                    },
                    { // k = 60
                            5, 302, 586, 1279, 2093, 2259, 2764, 2990, 4030, 5124,
                            5371, 5852, 6837, 6977, 7682, 7852, 7889, 7934, 8087, 10135,
                    },
                    { // k = 120
                            1527, 2473, 5522, 5525, 5664, 5845, 17812, 18265, 18323, 148071,
                            179390, 180588, 182642, 186015, 186723, 189611, 190333, 191069, 191156, 191187,
                    }
            }
    };

    vector<vector<int>> ks = {
            {3,5,10,15,20,25,30,35,40},
            {3,5,10,15,20,25,30,35,40,45,50},
            {3,5,10,15,20,25,30,35,40,45,50},
            {3,5,10,15,20,25,30,35,40,45,50},
    };

//    vector<int> Ns = {500, 1000, 1500, 2000};
    vector<int> rs = {10,20,30,40,50,60,70,80,90,100};

    base b;
    locals lo;
    pkcore pkc;

    clock_t start, end;
    double time;

    for(int i = 0; i < 1; i++){ // TIPS: 从email开始
        string file = files[i];
        string filename = "data/"+file;

        printf("Filename: %s\n", file.c_str());
        printf("%s: Reading graph ...\n", file.c_str());

        auto G = b.readFilep(filename);
        auto Edges = b.readFileE(filename);
        auto dG = b.toDeter(G);

        auto ks_of_this_file = ks[i];
        auto qs_of_this_file = qs[i];

        for(int l = 0; l < 1; l++) { //
            int k = ks_of_this_file[l]; // TIPS: k=3

            auto qs_of_this_k = qs_of_this_file[0]; // TIPS: 选择core_number=3的查询点

            unordered_map<int,vector<double>> r2time_ss_vl, r2time_ss_sc,
                    r2time_lssa_vl, r2time_lssa_sc;

            for (int ll = 0; ll < qn; ll++) {
                int q = qs_of_this_k[ll];

                auto Cqk = lo.global(q, k, dG);
                auto G_prune_i = lo.extractSubgraph(Cqk, G);
                G_prune_i = lo.rearrange(G_prune_i, q, Cqk);
                auto Edges_prune_i = lo.table2Edges(G_prune_i);
                auto dG_prune_i = b.toDeter(G_prune_i);

                vector<int> C;
                for (auto v : Cqk) {
                    if (pkc.degGeK(G_prune_i, v, k) >= theta) {
                        C.emplace_back(v);
                    }
                }

                printf("G size=%d, G_pruned size=%d, Edges size=%d, Edges_pruned size=%d, C size=%d\n",
                       (int) G.size(), (int) G_prune_i.size(), (int) Edges.size(), (int) Edges_prune_i.size(),
                       (int) C.size());

                vector<vector<double>> Prs;
                vector<double> Pr_v_avg;
                double var;

                printf("%s: Start evaluating ...\n", file.c_str());

                for(int j = 0; j < rs.size(); j++){
                    int r = rs[j];

                    // TIPS: SS-VL
                    Pr_v_avg.clear();
                    Prs.clear();
                    for (int lll = 0; lll < n0; lll++) {
                        printf("%s:  1/1 k, %d/%d q, %d/%d r, %d/%d loop, ss-vl\n",
                               file.c_str(),
                               ll + 1, qn,
                               j+1, (int)rs.size(),
                               lll + 1, n0);

                        vector<double> Pr_v;

                        start = clock();
                        ss_vl(G_prune_i, Edges_prune_i, C, q, k, theta, Pr_v, 500, r);
                        end = clock();
                        time = (double) (end - start) / CLOCKS_PER_SEC;

                        r2time_ss_vl[r].emplace_back(time);
                    }

                    // TIPS: SS-SC
                    Prs.clear();
                    Pr_v_avg.clear();
                    for (int lll = 0; lll < n0; lll++) {
                        printf("%s:  1/1 k, %d/%d q, %d/%d r, %d/%d loop, ss-sc\n",
                               file.c_str(),
                               ll + 1, qn,
                               j+1, (int)rs.size(),
                               lll + 1, n0);

                        vector<double> Pr_v;

                        start = clock();
                        ss_sc(G_prune_i, Edges_prune_i, dG_prune_i, C, q, k, theta, Pr_v, 500, r);
                        end = clock();
                        time = (double) (end - start) / CLOCKS_PER_SEC;

                        r2time_ss_sc[r].emplace_back(time);
                    }

                    // TIPS: LSSA-VL
                    Pr_v_avg.clear();
                    Prs.clear();
                    for (int lll = 0; lll < n0; lll++) {
                        printf("%s:  1/1 k, %d/%d q, %d/%d r, %d/%d loop, lssa-vl\n",
                               file.c_str(),
                               ll + 1, qn,
                               j+1, (int)rs.size(),
                               lll + 1, n0);

                        vector<double> Pr_v;

                        start = clock();
                        lssa_vl(G_prune_i, Edges_prune_i, C, q, k, theta, Pr_v, 500, r);
                        end = clock();
                        time = (double) (end - start) / CLOCKS_PER_SEC;

                        r2time_lssa_vl[r].emplace_back(time);
                    }

                    // TIPS: LSSA-SC
                    Pr_v_avg.clear();
                    Prs.clear();
                    for (int lll = 0; lll < n0; lll++) {
                        printf("%s:  1/1 k, %d/%d q, %d/%d r, %d/%d loop, lssa-sc\n",
                               file.c_str(),
                               ll + 1, qn,
                               j+1, (int)rs.size(),
                               lll + 1, n0);

                        vector<double> Pr_v;

                        start = clock();
                        lssa_sc(G_prune_i, Edges_prune_i, dG_prune_i, C, q, k, theta, Pr_v, 500, r);
                        end = clock();
                        time = (double) (end - start) / CLOCKS_PER_SEC;

                        r2time_lssa_sc[r].emplace_back(time);
                    }
                }
            }

            //

            ofstream out;

            // TIPS: 将方差结果写入文件, SS_VL
            printf("%s: Writing variances to file, ss-vl ... \n", file.c_str());
            out.open("output/ss_vl_variances_of_" + file, ios::app);
            out << "k=" << k << endl;
            for(auto [r, time_of_qs] : r2time_ss_vl){
                out<<"r="<<r;
                for (int j = 0; j < qn; j++) {
                    if (j % 10 == 0) out << endl;
                    out << time_of_qs[j] << " ";
                }
                out << endl;
            }
            out.close();

            // TIPS: 将方差结果写入文件, SS_SC
            printf("%s: Writing variances to file, ss-sc ... \n", file.c_str());
            out.open("output/ss_sc_variances_of_" + file, ios::app);
            out << "k=" << k << endl;
            for(auto [r, time_of_qs] : r2time_ss_sc){
                out<<"r="<<r;
                for (int j = 0; j < qn; j++) {
                    if (j % 10 == 0) out << endl;
                    out << time_of_qs[j] << " ";
                }
                out << endl;
            }
            out.close();

            // TIPS: 将方差结果写入文件, lssa-vl
            printf("%s: Writing variances to file, lssa-vl ... \n", file.c_str());
            out.open("output/lssa_vl_variances_of_" + file, ios::app);
            out << "k=" << k << endl;
            for(auto [r, time_of_qs] : r2time_lssa_vl){
                out<<"r="<<r;
                for (int j = 0; j < qn; j++) {
                    if (j % 10 == 0) out << endl;
                    out << time_of_qs[j] << " ";
                }
                out << endl;
            }
            out.close();

            // TIPS: 将方差结果写入文件, lssa-sc
            printf("%s: Writing variances to file, lssa-sc ... \n", file.c_str());
            out.open("output/lssa_sc_variances_of_" + file, ios::app);
            out << "k=" << k << endl;
            for(auto [r, time_of_qs] : r2time_lssa_sc){
                out<<"r="<<r;
                for (int j = 0; j < qn; j++) {
                    if (j % 10 == 0) out << endl;
                    out << time_of_qs[j] << " ";
                }
                out << endl;
            }
            out.close();
        }
    }
}