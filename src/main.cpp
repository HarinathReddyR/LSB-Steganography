#include <opencv2/opencv.hpp>
#include <iostream>
#include <bitset>
using namespace std;

bool check(const int img_size,const unsigned int r_size){
    return img_size*3>=r_size*8+32;
}

string string_to_binary(string s){
    string b="";
    for(int i=0;i<s.size();i++){
        char c=s[i];
        b+=bitset<8>(c).to_string();
    }
    return b;
}

void embed_data(cv::Mat &image,string &text,int bit){
    for(int i=0;i<(int)text.size();i++){
        int row = (bit/image.cols*3);
        int col = (bit % (image.cols*3))/3;
        cv::Vec3b& curr = image.at<cv::Vec3b>(row,col);
        uint8_t& curr_color = curr[bit%3];
        curr_color = ((curr_color&0xFE)|(text[i]-'0'));
        bit++;
    }
}
void embed_text(cv::Mat &image,string &t){
    int bit=0;
    int len=t.size();
    string len_text=bitset<32>(len).to_string();
    string text=string_to_binary(t);
    embed_data(image,len_text,0);
    embed_data(image,text,32);
}

unsigned int binary_to_decimal(string len_text){
    unsigned long long int val = std::bitset<32>(len_text).to_ulong();
    return val;
}
string extract_text(cv::Mat &image){
    string text="";
    int bit=0;
    string len_text="";
    for(int i=0;i<32;i++){
        int row = (bit/image.cols*3);
        int col = (bit % (image.cols*3))/3;
        cv::Vec3b& curr = image.at<cv::Vec3b>(row,col);
        uint8_t& curr_color = curr[bit%3];
        len_text+=(curr_color&1)+'0';
        bit++;
    }
    unsigned int ac_len=binary_to_decimal(len_text);
    // cout<<"len of text"<<ac_len<<'\n';
    string byte="";
    for(unsigned int  i=0;i<ac_len*8;i++){
        int row = (bit/image.cols*3);
        int col = (bit % (image.cols*3))/3;
        cv::Vec3b& curr = image.at<cv::Vec3b>(row,col);
        uint8_t& curr_color = curr[bit%3];
        byte+=(curr_color&1)+'0';
        bit++;
        if(byte.size()==8){
            unsigned long long int val = std::bitset<8>(byte).to_ulong();
            text+=static_cast<char>(val);
            // cout<<"text="<<text<<'\n';
            byte="";
        }

    } 
    return text;
}
int main() {
    // Load an image from file
    cv::Mat image = cv::imread("C:/Users/DELL/Desktop/LSB-Steganography/images/kakashi.jpeg"); 

    // Check if the image was loaded successfully
    if (image.empty()) {
        cerr << "Error: Unable to load image!" << endl;
        return -1;
    }

    string text="This is Secret";
    if(!check(image.rows *image.cols,text.size())){
        cerr<<"Text size is more!"<<'\n';
        return -1;
    }

    embed_text(image,text);
    cout<<"emnbeded"<<'\n';
    cv::imwrite("C:/Users/DELL/Desktop/LSB-Steganography/images/stegano_image.png", image);
    cout<<"image creatd"<<'\n';
    cout<<"extract_text= "<<extract_text(image);
    return 0;
}
