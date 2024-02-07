//
// Created by anouk on 24/01/2024.
//

#include <iostream>
#include "feature_size.hpp"

pair<int,int> feature_size::extract(const string& fileIm){
    Mat imBase = imread(fileIm);
    Mat im;
    cv::cvtColor(imBase, im, cv::COLOR_BGR2GRAY);
    threshold(im, im, 200, 255, cv::THRESH_BINARY);

    int premierPixelNoirX = im.cols;
    int dernierPixelNoirX = 0;
    int premierPixelNoirY = im.rows;
    int dernierPixelNoirY = 0;
    // parcourt les colonnes de l'image
    for (int x = 0; x < im.cols; ++x) {
        // parcourt les lignes de l'image
        for (int y = 0; y < im.rows; ++y) {
            if (im.at<uchar>(y, x) < 100) {  // pixel noir trouvé
                if (y<premierPixelNoirY) {
                    premierPixelNoirY = y;
                }
                if (y>dernierPixelNoirY) {
                    dernierPixelNoirY = y;
                }
                if (x<premierPixelNoirX) {
                    premierPixelNoirX = x;
                }
                if (x>dernierPixelNoirX) {
                    dernierPixelNoirX = x;
                }
            }
        }
    }

    // Calcul la largeur du motif
    int largeur = dernierPixelNoirX - premierPixelNoirX ;
    // Calcul la hauteur du motif
    int hauteur = dernierPixelNoirY - premierPixelNoirY ;
    std::cout<<"x :"<<premierPixelNoirX<<" x der : "<< dernierPixelNoirX<<"   y:"<<premierPixelNoirY<<" y der :"<<dernierPixelNoirY<<endl;

    Rect roiRect(premierPixelNoirX, premierPixelNoirY, largeur, hauteur);  // (x, y, width, height)
    Mat roi = imBase(roiRect).clone();
    int newHauteur;
    int newLargeur;
    if(hauteur>largeur){
        newHauteur=250;
        double ratio=(static_cast<double>(largeur)/hauteur)*250;
        newLargeur = static_cast<int>(ratio);
    } else {
        newLargeur = 250;
        double ratio = (static_cast<double>(hauteur)/largeur)*250;
        newHauteur = static_cast<int>(ratio);
    }
    cv::resize(roi, roi, cv::Size(newLargeur, newHauteur));

    // Image vide blanche pour les marges
    cv::Mat imVide(250, 250, roi.type(), cv::Scalar(255, 255, 255));
    // Position de l'image redimensionnée sur la vide
    int posX = static_cast<int>((250 - newLargeur) / 2);
    int posY = static_cast<int>((250 - newHauteur) / 2);
    cv::Rect rec = cv::Rect(posX, posY, newLargeur, newHauteur);
    // Place image redimensionnée sur le fond blanc
    roi.copyTo(imVide(rec));

    imwrite("../saved_model.png",imVide);

    return make_pair(largeur, hauteur);
}


