#include "../include/Texture.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;


Texture::Texture(const std::string& fileName) : width(0), height(0) {
    string format = fileName.substr(fileName.find('.')+1);

    if (format == "ppm") {
        ifstream file(fileName);
        string magic_number;
        int max_value;

        if (!file.is_open()) {
            cerr << "Erro: Não foi possível abrir o arquivo de textura " << fileName << endl;
            exit(EXIT_FAILURE);
            return;
        }
        
        file >> magic_number;
        if (magic_number != "P3") {
            cerr << "Erro: Apenas arquivos PPM P3 (ASCII) são suportados." << endl;
            exit(EXIT_FAILURE);
            return;
        }

        while (file.peek() == '#' || file.peek() == '\n' || file.peek() == ' ' || file.peek() == '\t') {
            if (file.peek() == '#') {
                file.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                file.ignore();
            }
        }

        file >> width >> height;
        file >> max_value;

        if (max_value != 255) {
            cerr << "Aviso: Valor máximo de cor diferente de 255. A textura pode não ser exibida corretamente." << endl;
        }

        data.resize(width * height * 3);

        for (int j = height - 1; j >= 0; --j) {
            for (int i = 0; i < width; ++i) {
                int r, g, b;
                file >> r >> g >> b;
                data[(j * width + i) * 3 + 0] = static_cast<unsigned char>(r);
                data[(j * width + i) * 3 + 1] = static_cast<unsigned char>(g);
                data[(j * width + i) * 3 + 2] = static_cast<unsigned char>(b);
            }
        }

        file.close();
    // } else if (format == "jpg") {
    //     int channels_in_file;
    //     int desired_channels = 3; // Forçar 3 canais (RGB)

    //     // A função stbi_load faz todo o trabalho: decodifica o JPG, aloca memória e retorna os dados
    //     unsigned char* img_data = stbi_load(
    //         fileName.c_str(),     // Caminho do arquivo
    //         &this->width,         // Ponteiro para a largura (a função preenche)
    //         &this->height,        // Ponteiro para a altura (a função preenche)
    //         &channels_in_file,    // Canais no arquivo original
    //         desired_channels      // Força a ter 3 canais (RGB)
    //     );

    //     // Verifica se a imagem foi carregada com sucesso
    //     if (img_data == nullptr) {
    //         cerr << "Erro: Não foi possível carregar a textura do arquivo " << fileName << endl;
    //         cerr << "Motivo: " << stbi_failure_reason() << endl;
    //         exit(EXIT_FAILURE);
    //     }

    //     // Copia os dados da imagem para o seu vetor 'data'
    //     size_t data_size = width * height * desired_channels;
    //     data.resize(data_size);
    //     memcpy(data.data(), img_data, data_size);

    //     // Libera a memória alocada pela biblioteca stbi
    //     stbi_image_free(img_data);
    } else {
        cerr << "ERROR: Formato de imagem inválido!" << endl;
        exit(EXIT_FAILURE);
    }
}


Vector Texture::value(pair<double, double> p) const {
    double u = p.first;
    double v = p.second;
    if (data.empty()) {
        return Vector{0, 1, 1};
    }

    while (u < 0) {
        u += 1.0;
    }
    while (u >= 1.0) {
        u -= 1.0;
    }

    while (v < 0) {
        v += 1.0;
    }
    while (v >= 1.0) {
        v -= 1.0;
    }

    int i = (int)(u * (width - 1));
    int j = (int)((1 - v) * (height - 1));

    int index = (j * width + i) * 3;

    double r = data[index + 0] / 255.0;
    double g = data[index + 1] / 255.0;
    double b = data[index + 2] / 255.0;

    return Vector{r, g, b};
}

pair<double, double> Texture::getTexelSize() const {
    return {1/width, 1/height};
}

void Texture::toPpm(const string& fileName) {
    ofstream exitRGB("img/" + fileName + ".ppm");

    exitRGB << "P3\n" << width << " " << height << "\n255\n";
    int progTotal = width * height * 3;
    for (int i = 0; i < progTotal; i+=3) {
        exitRGB << (size_t)data[i] << " " << (size_t)data[i+1] << " " << (size_t)data[i+2] << endl;
    }

    exitRGB.close();
    cout << "Arquivo '" << fileName << ".ppm' criado na pasta 'img' com sucesso!" << endl;

    return;
}