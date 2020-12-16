#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include <iomanip>

#define coutfixed cout << fixed << setprecision(0)

using namespace std;

int Int(string num) {
    int num_i = 0;

    while (num.size() > 0) {
        num_i *= 10;
        num_i += num[0] - '0';
        num = num.substr(1);
    }

    return num_i;
}

vector<string> File_To_Str_Vec(string filename) {
    vector<string> lines_int;
    string line;
    ifstream file;

    file.open(filename.c_str());

    getline(file, line);
    while (file) {
        lines_int.push_back(line);
        getline(file, line);
    }

    file.close();

    return lines_int;
}

template<typename type, typename sub_type>
vector<type> Split(type data, sub_type split_on) {
    vector<type> vec;
    type sub_data;

    for (int i = 0; i <= data.size(); i++) {
        if (i == data.size() || data[i] == split_on) {
            vec.push_back(sub_data);
            sub_data.clear();
        }
        else {
            sub_data += data[i];
        }
    }

    return vec;
}

void Get_Earliest(vector<string> buses) {
    int start = Int(buses[0]);
    vector<string> ids = Split(buses[1], ',');
    vector<int> times;
    int id;
    int time;
    int early_id = -1;
    int early_time = -1;
    int time_waiting;

    for (int i = 0; i < ids.size(); i++) {
        if (ids[i] != "x") {
            id = Int(ids[i]);

            time = (start / id) * id + id;

            if (early_id == -1 || time < early_time) {
                early_id = i;
                early_time = time;
            }
        }
    }

    time_waiting = early_time - start;

    cout << ids[early_id] << " * " << time_waiting << " = " << Int(ids[early_id]) * time_waiting << endl;
}

string String(int num, int depth = 0) {
    string str = "";
    int num2;

    if (num > 0) {
        num2 = (num % 10);
        return (char)(num2 + '0') + String(num / 10, ++depth);
    }
    else if (depth == 0) {
        return "0";
    }
    else {
        return "";
    }
}

string Simplified(double amount) {
    if (amount < 100) {
        return String(amount);
    }
    else if (amount < 100000) {
        return String(amount/100) + "K";
    }
    else if (amount < 100000000) {
        return String(amount/100000) + "M";
    }
}

int Get_Valid(vector<string> ids, double start) {
    int valid = 0;
    int id;
    double position;

    for (int i = 0; i < ids.size(); i++) {
        if (ids[i] != "x") {
            id = Int(ids[i]);
            position = start / id;

            if (position != trunc(start / id)) {
                position = trunc(position) * id + id;
            }
            else {
                position = trunc(position) * id;
            }

            while (start + i > position) {
                position += id;
            }

            id -= id;

            if (position == start + i) {
                valid++;
            }
            else {
                return valid;
            }
        }
        else {
            valid++;
        }
    }

    return valid;
}

double Multiply(vector<string> ids, int last_index) {
    double product = 1;

    for (int i = 0; i < last_index; i++) {
        if (ids[i] != "x") {
            product *= Int(ids[i]);
        }
    }

    return product;
}

double Get_Earliest2(vector<string> ids) {
    double start = (double)Int(ids[0]);
    double length = start;
    int num_valid = 0;
    int highest = 0;

    do {
        //coutfixed << "Mult: " << Multiply(ids, ids.size()) << endl;

        num_valid = Get_Valid(ids, start);
        if (num_valid > highest) {
            highest = num_valid;
            length = Multiply(ids, num_valid);

            coutfixed << start << " \t" << highest << "\t" << length << endl;
        }
        else {
            cout << start << "\t" << num_valid << endl;
        }

        start += length;

        //num_valid = ids.size();

    }
    while(num_valid != ids.size());

    start -= length;

    return start;
}

int main()
{
    vector<string> buses;
    double time;

    buses = File_To_Str_Vec("bus.txt");

    Get_Earliest(buses);

    time = Get_Earliest2(Split(buses[1], ','));

    cout << fixed << setprecision(0) << time << endl;

    return 0;
}
