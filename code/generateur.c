#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "structures.h"

#define MARGE 50 
#define TAILLEPOINT 8 //Diametre d'un point
int N, xmin , xmax, ymin, ymax, inf, sup;

double centerX, centerY, rayon;
double centerX_dessin, centerY_dessin, rayondessin; 
double res_x, res_y, res_rayon;
double res_x_dessin, res_y_dessin, res_rayon_dessin;

void findCircle_deux_points(POINT p1, POINT p2, double *centerX, double *centerY, double *rayon,double *centerX_dessin, double *centerY_dessin, double *rayondessin) {
    // Calcul des coordonnées du centre potentiel du cercle
    *centerX = (p1.x + p2.x) /2;
    *centerY = (p1.y + p2.y) /2;
    
    *rayon = sqrt(pow(*centerX - p1.x, 2) + pow(*centerY - p1.y, 2));
    // Calcul du rayon du cercle
    *centerX_dessin = ((*centerX-inf)*1800/(sup-inf))+100;
    *centerY_dessin = ((*centerY-inf)*1800/(sup-inf))+100;
    double n_pix = ((p1.x-inf)*1800/(sup-inf))+100;
    double n_piy = ((p1.y-inf)*1800/(sup-inf))+100;
    *rayondessin = sqrt(pow(*centerX_dessin - n_pix, 2) + pow(*centerY_dessin - n_piy, 2));
}

// Fonction pour calculer le centre et le rayon du cercle passant par trois points
void findCircle(POINT p1, POINT p2, POINT p3, double *centerX, double *centerY, double *rayon, double *centerX_dessin, double *centerY_dessin, double *rayondessin) {
    // Calcul des coordonnées du centre potentiel du cercle
    double m1 = (p2.y - p1.y) / (p2.x - p1.x);
    double m2 = (p3.y - p2.y) / (p3.x - p2.x);

    *centerX = (m1 * m2 * (p3.y - p1.y) + m1 * (p2.x + p3.x) - m2 * (p1.x + p2.x)) / (2 * (m1 - m2));
    *centerY = -1 * (*centerX - (p1.x + p2.x) / 2) / m1 + (p1.y + p2.y) / 2;
    // Calcul du rayon du cercle
    *rayon = sqrt(pow(*centerX - p1.x, 2) + pow(*centerY - p1.y, 2));
    *centerX_dessin = ((*centerX-inf)*1800/(sup-inf))+100;
    *centerY_dessin = ((*centerY-inf)*1800/(sup-inf))+100;
    double n_pix = ((p1.x-inf)*1800/(sup-inf))+100;
    double n_piy = ((p1.y-inf)*1800/(sup-inf))+100;
    *rayondessin = sqrt(pow(*centerX_dessin - n_pix, 2) + pow(*centerY_dessin - n_piy, 2));
    
}

//Dessine un point en SVG
char* dessinerPoint(FILE *file, POINT p, int r){ 
  char* codePoint= malloc (sizeof (*codePoint) * 500);
  sprintf(codePoint,"<circle cx=\"%f\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"black\"/>",p.x,p.y,r);
  fprintf(file,"%s\n", codePoint);
  return codePoint;
}

char* dessinerCercle(FILE *file, double x, double y, double r){
  char* codeCercle= malloc (sizeof (*codeCercle) *500);
  sprintf(codeCercle,"<circle cx=\"%f\" cy=\"%f\" r=\"%f\" stroke=\"red\" stroke-width=\"3\" fill=\"transparent\" fill-opacity=\"0\" />",x,y,r);
  fprintf(file,"%s\n", codeCercle);
  return codeCercle;
}

int verification (POINT* tab, double *centerX, double *centerY, double *rayon){
  for (int i = 0; i < N; i++) {
    POINT p;
    p.x = tab[i].x;
    p.y = tab[i].y;
    double distance = sqrt(pow(*centerX - p.x, 2) + pow(*centerY - p.y, 2));
    if (distance > *rayon) {
      return 0;
    }
  }
  return 1;
}
void algo_naif (FILE *file,POINT* tab, int N){
  for (int i = 0; i < N-1; i++) {
    POINT p;
    p.x = tab[i].x;
    p.y = tab[i].y;
    for (int j = i+1; j < N; j++) {
      POINT q;
      q.x = tab[j].x;
      q.y = tab[j].y;
      printf("%2f , %.2f \n", p.x, p.y);
      printf("%.2f , %.2f \n", q.x, q.y);
      findCircle_deux_points(p, q, &centerX, &centerY, &rayon, &centerX_dessin, &centerY_dessin, &rayondessin);
      //verifiation des points 
      if (verification(tab, &centerX, &centerY, &rayon)== 1){
          dessinerCercle(file,centerX_dessin,centerY_dessin,rayondessin);
          return ; 
      }
    }
  }
  
  for (int i = 0; i < N-2; i++) {
    POINT p;
    p.x = tab[i].x;
    p.y = tab[i].y;
    for (int j = i+1; j < N-1; j++) {
      POINT q;
      q.x = tab[j].x;
      q.y = tab[j].y;
      for (int k = j+1; k < N; k++) {
        POINT r;
        r.x = tab[k].x;
        r.y = tab[k].y;
        double d = (p.x * (q.y - r.y) + q.x * (r.y - p.y) + r.x * (p.y - q.y)) * 2 ;
        if (d==0){
          printf("Pas de cercle possible passant par ces 3 points");
        }else{
          printf("%2f , %.2f \n", p.x, p.y);
          printf("%.2f , %.2f \n", q.x, q.y);
          printf("%.2f , %.2f \n", r.x, r.y);
          findCircle(p, q, r, &centerX, &centerY, &rayon,&centerX_dessin, &centerY_dessin,&rayondessin);
          printf("centreX %.2f, centreY %.2f, rayon %.2f \n", centerX, centerY, rayon);
          if (verification(tab, &centerX, &centerY, &rayon)== 1 && rayon<res_rayon){
              res_x = centerX;
              res_y = centerY;
              res_rayon = rayon; 
              res_x_dessin = centerX_dessin;
              res_y_dessin = centerY_dessin;
              res_rayon_dessin = rayondessin;
          }
        }
      }
    }
  }
  dessinerCercle(file, res_x_dessin, res_y_dessin, res_rayon_dessin);
}

void trouver_c (POINT tab[],FILE* file,int N, double *centerX, double *centerY, double *rayon){
  if(N < 2){
    centerX = 0; 
    centerY = 0; 
    rayon = 0; 
  }else{
    algo_naif(file,tab,N);
  }
}


//Dessine les points et le cercle dans le SVG
void ecritureSVG(POINT tab[], FILE* file , int N){
  for (int i = 0; i < N; i++) {
    POINT p;
    p.x = ((tab[i].x-inf)*1800/(sup-inf))+100;
    p.y = ((tab[i].y-inf)*1800/(sup-inf))+100;
    dessinerPoint(file, p, TAILLEPOINT);
  }
  trouver_c(tab,file,N,&centerX,&centerY,&rayon);
}

void GenerationFichierSVG(POINT* tab , int N){
  //creation et ouverture du fichier
  FILE *file;
  file= fopen("Points.svg", "w");
  
  //ecriture de l'entete

  fprintf(file,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
  fprintf(file,"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n");
  fprintf(file,"\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
  fprintf(file,"<svg width=\"2000\" height=\"2000\" version=\"1.1\"\n");
  fprintf(file, "xmlns=\"http://www.w3.org/2000/svg\" style=\"background-color: white\">\n");
  fprintf(file, "<rect x=\"0\" y=\"0\" width=\"2000\" height=\"2000\" fill=\"#ffffff\" />\n");

  fprintf(file, "<line x1=\"100\" y1=\"100\" x2=\"1900\" y2=\"100\" stroke=\"black\" />\n"); // Axe x

  fprintf(file, "<text x=\"1900\" y=\"100\" font-family=\"Arial\" font-size=\"20\">%d</text>\n", sup);

  fprintf(file, "<line x1=\"100\" y1=\"100\" x2=\"100\" y2=\"1900\" stroke=\"black\" />\n"); // Axe y
  fprintf(file, "<text x=\"100\" y=\"100\" font-family=\"Arial\" font-size=\"20\">%d</text>\n", inf);
  fprintf(file, "<text x=\"100\" y=\"1900\" font-family=\"Arial\" font-size=\"20\">%d</text>\n", sup);    

 
  //ecriture du programme
  ecritureSVG(tab,file,N);
  //fin du programme et fermer le fichier
  fprintf(file, "</svg>\n");
  fclose(file);
}

int rand_a_b (int a, int b){
  return rand()%(b-a )+a;
}

///////////////////////////////WELZ//////////////////////////
double distance(POINT p1, POINT p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

int pointInsideCircle(POINT p, CERCLE c) {
    POINT q; 
    q.x = c.x_cercle;
    q.y = c.y_cercle;
    return distance(p, q) <= c.rayon_cercle;
}

CERCLE welz(POINT* tab, int n){
    CERCLE res;
    if (n == 0) {
        res.x_cercle = 0; 
        res.y_cercle = 0; 
        res.rayon_cercle = 0;
        return (res);
    }
    // Cas de base : un point
    else if (n == 1) {
        res.x_cercle = tab[0].x; 
        res.y_cercle = tab[0].y; 
        res.rayon_cercle = 0;
        return (res);
    }
    // Cas de base : deux points
    else if (n == 2) {
        res.x_cercle = (tab[0].x + tab[1].x) / 2;
        res.y_cercle = (tab[0].y + tab[1].y) / 2;
        res.rayon_cercle = sqrt(pow(tab[1].x - tab[0].x, 2) + pow(tab[1].y - tab[1].y, 2));
        return (res);
    }
    // Cas général
    else {
        // Sélectionnez un point aléatoire
        int randomIndex = rand() % n;
        POINT randomPoint = tab[randomIndex];

        // Supprimez ce point de la liste
        for (int i = randomIndex; i < n - 1; ++i) {
            tab[i] = tab[i + 1];
        }

        // Appel récursif pour les points restants
        CERCLE minCircle = welz(tab, n - 1);

        // Si le point n'est pas dans le cercle, agrandissez le cercle pour l'inclure
        if (!pointInsideCircle(randomPoint, minCircle)) {
            // Créez un cercle avec le point comme seul point du contour
            minCircle.x_cercle = randomPoint.x;
            minCircle.y_cercle = randomPoint.y;
            minCircle.rayon_cercle = 0;

            // Trouvez le cercle minimum contenant les points restants
            for (int i = 0; i < n - 1; ++i) {
                if (!pointInsideCircle(tab[i], minCircle)) {
                    // Sélectionnez deux points
                    minCircle.x_cercle = (tab[i].x + randomPoint.x) / 2;
                    minCircle.y_cercle = (tab[i].y + randomPoint.y) / 2;
                    minCircle.rayon_cercle = distance(tab[i], randomPoint) / 2;
                    for (int j = 0; j < i; ++j) {
                        if (!pointInsideCircle(tab[j], minCircle)) {
                            // Calculez le cercle contenant les trois points
                            minCircle.x_cercle = (tab[i].x + tab[j].x) / 2;
                            minCircle.y_cercle = (tab[i].y + tab[j].y) / 2;
                            minCircle.rayon_cercle = distance(tab[i], tab[j]) / 2;
                            break;
                        }
                    }
                }
            }
        }

        return minCircle;
    }


}


int main( int argc, char* argv []){
   //N=nb  de points et coordonnées du cadre 
  srand(time(NULL));
  if (argc!=6){
    printf ("nb invalide d'arguments\n");
    printf("usage: %s <nbDePoints> <xmin> <xmax> <ymin> <ymax>\n", argv[0]);
    return 1;
  }
  N=atoi(argv[1]);
  printf ("%d \n", N); //Affichage nb de points générés
  xmin=atoi(argv[2]);   
  xmax=atoi(argv[3]);
  ymin=atoi(argv[4]);
  ymax=atoi(argv[5]);
  int i;
  int x, y; 
  res_rayon = INFINITY;
  inf = xmin;
  sup = xmax; 
  if (ymax>sup){
    sup = ymax;
  }
  if (ymin<inf){
    inf = ymin;
  }
  POINT* tab=malloc (sizeof(POINT)*N);
  for (i=0;i<N;i++){
    if (xmin != xmax){
      x=rand_a_b(xmin,xmax);
    }else{
      x = xmin;
    } 
    if (ymin != ymax){
      y=rand_a_b(ymin,ymax);
    }else{
      y = ymin;
    } 
    tab[i].x= x;
    tab[i].y= y;
    printf("%d , %d \n", x,y);
  }
  if (inf == sup ){
    inf = sup - 1; 
    sup = sup+1;
  }
  GenerationFichierSVG(tab , N);
 
 //Implementation WELZ 

  CERCLE res_welz = welz(tab,N);
  printf("Centre du cercle minimum : (%.2f, %.2f)\n", res_welz.x_cercle, res_welz.y_cercle);
  printf("Rayon du cercle minimum : %.2f\n", res_welz.rayon_cercle);
  return 0; 
}