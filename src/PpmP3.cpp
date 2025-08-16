#include <string>
#include <array>
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

class PpmP3 {
private:
public:
    static void makeBuffer(string pattern, array<unsigned, 3> background = {0,0,0}, array<unsigned, 3> mainColour = {255,255,255}, string fileName = "myPPM", unsigned height = 100, unsigned width = 100, int squareSideSize = 0) {
        if (squareSideSize == 0) {
            squareSideSize = 1;
        }
        string fileBuffer = "";
        fileBuffer += "P3\n"+to_string(height)+" "+to_string(width)+"\n"+"255\n";

        if (pattern == "chess") {
            for (unsigned i = 0; i < height; ++i) {
                for (unsigned j = 0; j < width; ++j) {
                    unsigned columns = j / squareSideSize;
                    unsigned lines = i / squareSideSize;
                    if ((lines % 2) == (columns % 2)) {
                        fileBuffer += to_string(mainColour[0]) + " " + to_string(mainColour[1]) + " " + to_string(mainColour[2]) + " ";
                    } else {
                        fileBuffer += to_string(background[0]) + " " + to_string(background[1]) + " " + to_string(background[2]) + " ";
                    }
                    fileBuffer += "\n";
                }
            }
        }

        if (pattern == "zebra") {
            for (unsigned i = 0; i < height; ++i) {
                for (unsigned j = 0; j < width; ++j) {
                    if (((i + j) / squareSideSize) % 2 == 0) {
                        fileBuffer += to_string(mainColour[0]) + " " + to_string(mainColour[1]) + " " + to_string(mainColour[2]) + " ";
                    } else {
                        fileBuffer += to_string(background[0]) + " " + to_string(background[1]) + " " + to_string(background[2]) + " ";
                    }
                }
                fileBuffer += "\n";
            }
        }

        if (pattern == "diamond") {
            for (unsigned i = 0; i < height; ++i) {
                for (unsigned j = 0; j < width; ++j) {
                    int patternX = j % (2 * squareSideSize);
                    int patternY = i % (2 * squareSideSize);
                    int distance = abs(patternX - squareSideSize) + abs(patternY - squareSideSize);
                    if (distance <= squareSideSize) {
                        fileBuffer += to_string(mainColour[0]) + " " + to_string(mainColour[1]) + " " + to_string(mainColour[2]) + " ";
                    } else {
                        fileBuffer += to_string(background[0]) + " " + to_string(background[1]) + " " + to_string(background[2]) + " ";
                    }
                    fileBuffer += "\n";
                }
            }
        }

        if (pattern == "snake") {
            for (unsigned i = 0; i < height; ++i) {
                for (unsigned j = 0; j < width; ++j) {
                    // 1. Calcula um deslocamento horizontal usando a função seno.
                    //    Isso cria a curva suave da "cobra".
                    double frequency = 2.0 * M_PI / height; // Faz uma onda S completa na altura da imagem
                    double amplitude = squareSideSize * 1.5; // Controla a "largura" da ondulação
                    double offset = amplitude * sin(static_cast<double>(i) * frequency);

                    // 2. Aplica o deslocamento à coluna 'j' para obter uma posição "efetiva".
                    int effective_j = j + static_cast<int>(offset);

                    // 3. Cria as listras verticais com base na posição efetiva.
                    if ((effective_j / squareSideSize) % 2 == 0) {
                        fileBuffer += to_string(mainColour[0]) + " " + to_string(mainColour[1]) + " " + to_string(mainColour[2]) + " ";
                    } else {
                        fileBuffer += to_string(background[0]) + " " + to_string(background[1]) + " " + to_string(background[2]) + " ";
                    }
                }
                fileBuffer += "\n";
            }
        }

        if (pattern == "rings") {
            for (int i = 0; i < (int)height; ++i) {
                for (int j = 0; j < (int)width; ++j) {
                    // Garanta que o centro seja calculado com a divisão por 2
                    int centerX = width / 2;
                    int centerY = height / 2;
                    
                    // A distância deve ser calculada a partir do centro
                    double distance = sqrt(pow(j - centerX, 2) + pow(i - centerY, 2));

                    if ((static_cast<int>(distance) / squareSideSize) % 2 == 0) {
                        fileBuffer += to_string(mainColour[0]) + " " + to_string(mainColour[1]) + " " + to_string(mainColour[2]) + " ";
                    } else {
                        fileBuffer += to_string(background[0]) + " " + to_string(background[1]) + " " + to_string(background[2]) + " ";
                    }
                }
                fileBuffer += "\n";
            }
        }

        if (pattern == "bricks") {
            for (unsigned i = 0; i < height; ++i) {
                for (unsigned j = 0; j < width; ++j) {
                    unsigned line = i / squareSideSize;
                    unsigned column = j / squareSideSize;

                    if (line % 2 != 0) {
                        column = (j + squareSideSize / 2) / squareSideSize;
                    }

                    if (column % 2 == 0) {
                        fileBuffer += to_string(mainColour[0]) + " " + to_string(mainColour[1]) + " " + to_string(mainColour[2]) + " ";
                    } else {
                        fileBuffer += to_string(background[0]) + " " + to_string(background[1]) + " " + to_string(background[2]) + " ";
                    }
                }
                fileBuffer += "\n";
            }
        }

        ofstream exitRGB("img/" + fileName + ".ppm");
        if (exitRGB.is_open()) {
            exitRGB << fileBuffer;
            exitRGB.close();
        } else {
            cerr << "Erro: Nao foi possivel abrir o arquivo para escrita." << endl;
        }

        return;
    }

    static void makeHeights(string fileName = "myPPM") {
        ifstream inputFile(fileName);
        if (!inputFile.is_open()) {
            cerr << "Erro: Nao foi possivel abrir o arquivo de entrada: " << fileName << endl;
            return;
        }
        string fileNameHeights = fileName.insert(fileName.find('.'), "Heights");

        // Le o cabecalho do arquivo PPM
        string magic_number;
        int width, height, max_value;
        inputFile >> magic_number >> width >> height >> max_value;

        ofstream outputFile(fileNameHeights);
        if (!outputFile.is_open()) {
            cerr << "Erro: Nao foi possivel criar o arquivo de saida: " << fileNameHeights << endl;
            inputFile.close();
            return;
        }

        // Escreve o cabecalho do novo arquivo PPM em tons de cinza
        outputFile << "P3\n" << width << " " << height << "\n255\n";

        // Le os pixels da imagem original, converte para tons de cinza e escreve no novo arquivo
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int r, g, b;
                inputFile >> r >> g >> b;

                // Simples conversao para tons de cinza (media dos canais RGB)
                // int gray_value = (r + g + b) / 3;
                int gray_value = r;

                // Para criar um relevo simples, podemos usar o valor de brilho da imagem.
                // Para imagens com muitas cores, isso pode nao ser o ideal, mas funciona para texturas simples.
                // Outra forma seria pegar um canal especifico (por exemplo, o 'R').
                
                outputFile << gray_value << " " << gray_value << " " << gray_value << "\n";
            }
        }

        inputFile.close();
        outputFile.close();
        cout << "Mapa de altura gerado com sucesso em: " << fileNameHeights << endl;
    }
};