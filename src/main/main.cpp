#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include "lib/MjpegStream.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

void calcBoardCornerPositions(cv::Size boardSize, float squareSize, std::vector<cv::Point3f> &corners) {
  corners.clear();

  for (int i = 0; i < boardSize.height; ++i) {
    for (int j = 0; j < boardSize.width; ++j) {
      corners.push_back(cv::Point3f(float(j * squareSize), float(i * squareSize), 0));
    }
  }
}

void writeMat(cv::Mat &m, std::ofstream &fileOut) {
  fileOut << m.rows << " " << m.cols << " " << m.type() << std::endl;

  for (int i = 0; i < m.rows; i++) {
    for (int j = 0; j < m.cols; j++) {
      fileOut << m.at<double>(i, j) << "\t";
    }
    fileOut << std::endl;
  }
}

void readMat(cv::Mat &m, std::ifstream &fin) {
  int r, c, t;
  fin >> t >> c >> r;
  m = cv::Mat(r, c, t);

  for (int i = 0; i < m.rows; i++) {
    for (int j = 0; j < m.cols; j++) {
      fin >> m.at<double>(i, j);
    }
  }
}

#ifndef RUNNING_FRC_TESTS
int main() {
  // Collect Calibration data
  std::cout << "camera ID (name):" << std::endl;
  std::string cameraID;
  std::cin >> cameraID;

  cv::Size boardSize;

  std::cout << "Along WIDTH chessboard box number - 1 : " << std::endl;
  std::cin >> boardSize.width;

  std::cout << "Along HEIGHT chessboard box number - 1: " << std::endl;
  std::cin >> boardSize.height;

  std::cout << "Enter number of shoots : " << std::endl;
  int shootsNumber;
  std::cin >> shootsNumber;

  // Create http stream
  MjpegStream stream("Calibration", SCREEN_WIDTH, SCREEN_HEIGHT, 30);

  // Open capture and set properties
  cv::VideoCapture camera(0);
  camera.set(cv::CAP_PROP_FRAME_WIDTH, SCREEN_WIDTH);
  camera.set(cv::CAP_PROP_FRAME_HEIGHT, SCREEN_HEIGHT);

  // Objets utiles pour la calibration
  std::vector<std::vector<cv::Point3f>> object_points;
  std::vector<std::vector<cv::Point2f>> image_points;
  std::vector<cv::Point3f>              obj;
  std::vector<cv::Point2f>              corners;

  calcBoardCornerPositions(boardSize, 2.9f, obj);

  // Objets utiles pour la detection du chessboard
  cv::Mat image;
  cv::Mat gray_image;

  // Detection des chessboard : shootsNumber = nombre de chessboard a detecter
  for (int shoot = 1; shoot <= shootsNumber; shoot++) {
    // Tant que l'utilisateur n'a pas valide l'image
    for (bool success = false; !success;) {
      // Tant que le chessboard n'a pas été detecte 25 fois consecutives
      for (int counter = 0; counter < 25;) {
        // Si l'image peut etre lue
        if (camera.read(image)) {
          // Convertion en nuances de gris
          cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

          // Recherche du chessboard
          bool found = cv::findChessboardCorners(gray_image, boardSize, corners,
                                                 cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FILTER_QUADS);

          // Si le chessboard a ete detecte, on incremente le conteur et on le dessine
          // Sinon, on remet le compteur a zero
          if (found) {
            counter++;
            std::cout << "ChestboardCorner Found ! Counter = " << counter << std::endl;
            cv::cornerSubPix(gray_image, corners, cv::Size(11, 11), cv::Size(-1, -1),
                             cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 10, 0.1));
            cv::drawChessboardCorners(image, boardSize, corners, found);
          } else {
            counter = 0;
          }

          // Affichage de la photo
          stream.PutFrame(image);
#ifdef __DESKTOP__
          cv::imshow("Image", image);
          cv::waitKey(30);
#endif
        }
      }

      // On demande a l'utilisateur si il souhaite garder l'image
      std::cout << "Garder ? (o) ou Annuler (n) : ";
      std::string choix;
      std::cin >> choix;

      // Si le choix est affirmatif, les donnees sont sauvegardees et success = true pour sortir de la boucle
      // Sinon, success reste a false et on reste dans la boucle
      if (choix == "o") {
        success = true;
        image_points.push_back(corners);
        object_points.push_back(obj);
        std::cout << "Shoot " << shoot << "/" << shootsNumber
                  << " stored! [size of corners vector:" << corners.size() << "]" << std::endl;
      } else {
        std::cout << "Annulation capture !" << std::endl;
      }
    }
  }

  // CALIBRATION
  cv::Mat              intrinsic = cv::Mat(3, 3, CV_64FC1);
  cv::Mat              distCoeffs;
  std::vector<cv::Mat> rvecs;
  std::vector<cv::Mat> tvecs;

  cv::calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);

  // ECRITURE DANS FICHIER
  std::string filename = "calibration_camera_" + cameraID + "_" + std::to_string(SCREEN_WIDTH) + "x" +
                         std::to_string(SCREEN_HEIGHT) + ".txt";

  // Déclaration du flux et ouverture du fichier
  std::ofstream fileOut(filename, std::ios::out | std::ios::trunc);

  if (!fileOut) {
    std::cerr << "Erreur à l'ouverture !" << std::endl;
  } else {
    // Ecriture taille écran
    fileOut << SCREEN_WIDTH << " " << SCREEN_HEIGHT << std::endl;

    // Ecriture Data
    writeMat(intrinsic, fileOut);
    writeMat(distCoeffs, fileOut);

    fileOut << rvecs.size() << std::endl;
    for (int i = 0; i < rvecs.size(); i++) {
      writeMat(rvecs[i], fileOut);
    }

    fileOut << tvecs.size() << std::endl;
    for (int i = 0; i < tvecs.size(); i++) {
      writeMat(tvecs[i], fileOut);
    }

    // Ecriture Pied de page
    fileOut << "# ROBO'LYON CAMERA CALIBRATION DATA" << std::endl;
    fileOut << "# Video Reference Screen Size:" << SCREEN_WIDTH << "x" << SCREEN_HEIGHT << std::endl;

    // On referme le fichier
    fileOut.close();

    // On affiche l'endroit ou est situe le fichier de calibration
    std::cout << "Calibration file located in : " << std::experimental::filesystem::absolute(filename)
              << std::endl;
  }

  return 0;
}
#endif
