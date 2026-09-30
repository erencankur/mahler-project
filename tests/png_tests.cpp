#include <png.h>
#include <array>
#include <iostream>
#include <string>
#include <vector>

// Decode the serialized image, then compare colors and orientation against
// direct text search, trial-division primality, and an independent spiral walk.
int main(int argc,char** argv) {
    if (argc!=2) return 1;
    png_image image{}; image.version=PNG_IMAGE_VERSION;
    if (!png_image_begin_read_from_file(&image,argv[1])) return 1;
    image.format=PNG_FORMAT_RGB;
    std::vector<unsigned char> pixels(PNG_IMAGE_SIZE(image));
    if (!png_image_finish_read(&image,nullptr,pixels.data(),0,nullptr)) { png_image_free(&image); return 1; }
    if (image.width!=11 || image.height!=11) { png_image_free(&image); return 1; }
    png_image_free(&image);
    std::string prefix;
    for (unsigned n=1;n<=100;++n) prefix+=std::to_string(n);
    int x=0,y=0,direction=0,used=0,run=1,legs=0;
    constexpr int dx[]{1,0,-1,0},dy[]{0,1,0,-1};
    std::vector<bool> visited(121);
    for (unsigned n=1;n<=100;++n) {
        std::string before;
        for (unsigned a=1;a<n;++a) before+=std::to_string(a);
        const bool early=prefix.find(std::to_string(n))<before.size();
        bool prime=n>=2;
        for (unsigned a=2;a<n;++a) if (n%a==0) prime=false;
        const std::array<unsigned char,3> expected=early ?
            (prime?std::array<unsigned char,3>{0,158,115}:std::array<unsigned char,3>{0,114,178}) :
            (prime?std::array<unsigned char,3>{213,94,0}:std::array<unsigned char,3>{230,230,230});
        const auto index=static_cast<std::size_t>((5-y)*11+5+x); visited[index]=true;
        for (std::size_t c=0;c<3;++c) if (pixels[index*3+c]!=expected[c]) {
            std::cerr << "pixel mismatch at " << n << '\n'; return 1;
        }
        x+=dx[direction]; y+=dy[direction];
        if (++used==run) { used=0; direction=(direction+1)%4; if (++legs%2==0) ++run; }
    }
    for (std::size_t i=0;i<visited.size();++i) if (!visited[i])
        for (std::size_t c=0;c<3;++c) if (pixels[i*3+c]!=64) return 1;
    std::cout << "PNG pixels, orientation, early colors, and outside cells verified\n";
}
