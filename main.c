#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <string.h>
#include <time.h>
#include <SDL_mixer.h>

Uint32 SDL_GetTicks(void);
Uint32 avancerCaisse(Uint32 intervalle, void *parametre);
Uint32 avancerObjectif(Uint32 intervalle, void *parametre);
Uint32 avancerCaisse2(Uint32 intervalle, void *parametre);
int deplacement(int enMarche);
void ecrireScore();
int lectureBestScore();


int main(int argc, char *argv[])
{
    SDL_Surface *ecran = NULL, *Mario = NULL, *caisse1 = NULL, *score = NULL, *objectif = NULL, *caisse2 = NULL, *background = NULL, *gameOver = NULL, *texteJouer = NULL, *bestScore = NULL, *backgroundFloor = NULL, *backgroundCloud = NULL, *texteNom = NULL;

       TTF_Font *police = NULL, *policeGameOver = NULL, *policeNom = NULL;

    SDL_Rect positionMario, positionCaisse, positionScore, positionObjectif, positionCaisse2, positionBackground, positionGameOver, positionTexteJouer, positionBestScore, positionBackgroundFloor, positionBackgroundCloud, positionTexteNom;

        FILE*  fichier = NULL;
		fichier = fopen("score.txt", "r");


		if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
   {
      printf("%s", Mix_GetError());
   }

		Mix_Music *musiqueBackground, *musiquePiece, *musiqueGameOver, *musiqueSaut, *musiqueBoum;
		Mix_AllocateChannels(5);
		musiqueBackground = Mix_LoadMUS("sound/smb11.mp3");
		musiquePiece = Mix_LoadWAV("sound/piece.wav");
		musiqueGameOver = Mix_LoadWAV("sound/game-over.wav");
		musiqueSaut = Mix_LoadWAV("sound/saut.wav");
		musiqueBoum = Mix_LoadWAV("sound/Boum.wav");




    int nombreAlleatoire, nombreAlleatoire2, nombreAlleatoire3;
    int MAX = 1200, MIN = 640; //minimum et maximum pour l'alleatoire de la caisse
    int MAX2 = 2500, MIN2 = 640; //minimum et maximum pour l'alleatoire de l'objectif

    srand(time(NULL));
    nombreAlleatoire = (rand() % (MAX -MIN + 1)) + MIN; //nombre alleatoire pour position X de la caisse
    nombreAlleatoire2 = (rand() % (MAX2 -MIN2 + 1)) + MIN2; //nombre alleatoire pour position x de l'objectif
    nombreAlleatoire3 = (rand() % (MAX - MIN + 1)) + MIN; //nombre alleatoire pour position x de la caisse 2

    positionMario.x = 80;
    positionMario.y = 395;

    positionCaisse.x = nombreAlleatoire;
    positionCaisse.y = 447;

    positionScore.x = 100;
    positionScore.y = 0;

    positionObjectif.x = nombreAlleatoire2;
    positionObjectif.y = 446;

    positionCaisse2.x = nombreAlleatoire3;
    positionCaisse2.y = 447;

    positionBackground.x = 0;
    positionBackground.y = 0;

    positionGameOver.x = 640 / 3 - 50;
    positionGameOver.y = 480 / 2;

    positionTexteJouer.x = 640 / 3;
    positionTexteJouer.y = 480 / 2;

    positionBestScore.x = 300;
    positionBestScore.y = 0;

    positionBackgroundFloor.x = 20;
    positionBackgroundFloor.y = 480;

    positionBackgroundCloud.x = 0;
    positionBackgroundCloud.y = 0;

    positionTexteNom.x = 0;
    positionTexteNom.y = 452;


    int scoreNombre = 0;

    char scoreChar[20] = "";
    char texteJouerChar[20] = "Jouer";
    char texteNomChar[20] = "Clément Hennet";
    char gameOverChar[10] = "Game Over";
    char BestScoreAffiche[10];
    char texteBestScore[100];


    sprintf(scoreChar, "Score : %d", scoreNombre);


    int meilleureScore = lectureBestScore();

    sprintf(texteBestScore, "BestScore : %d", meilleureScore);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    TTF_Init();

    police = TTF_OpenFont("police/arial.ttf", 40);
    policeGameOver = TTF_OpenFont("police/arial.ttf", 65);
    policeNom = TTF_OpenFont("police/arial.ttf", 25);
    SDL_Color couleurBlanche = {255,255,255};


    score = TTF_RenderText_Blended(police, scoreChar, couleurBlanche);
    gameOver = TTF_RenderText_Blended(policeGameOver, gameOverChar, couleurBlanche);
    texteJouer = TTF_RenderText_Blended(policeGameOver, texteJouerChar, couleurBlanche);
    texteNom = TTF_RenderText_Blended(policeNom, texteNomChar, couleurBlanche);
    bestScore = TTF_RenderText_Blended(police, texteBestScore, couleurBlanche);

    ecran = SDL_SetVideoMode(640, 544, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("LineRunner", NULL);

    Mario = IMG_Load("img/mario1.png");
    caisse1 = IMG_Load("img/caisse.jpg");
    caisse2 = IMG_Load("img/caisse.jpg");
    objectif = IMG_Load("img/objectif.png");
    background = IMG_Load("img/background.png");
    backgroundFloor = IMG_Load("img/floor.png");
    backgroundCloud = IMG_Load("img/backgroundCloud.png");

        int continuer = 0, jouer = 1;
        int gravite = 0;
        int mort = 0;

        int temps = 1000, tempsActuel = 0, ancienTemps = 0;
        int temps2 = 3000, tempsActuel2 = 0, ancienTemps2 = 0;

        SDL_TimerID timer, timer2, timerObjectif;

        int VitesseCaisse = 40;

        timer = SDL_AddTimer(VitesseCaisse, avancerCaisse, &positionCaisse);
        timerObjectif = SDL_AddTimer(50, avancerObjectif, &positionObjectif);
        timer2 = SDL_AddTimer(VitesseCaisse, avancerCaisse2, &positionCaisse2);

        fclose(fichier);

        int img_joueur = 0;
        int img_floor = 0;
        int img_cloud = 0;
        int img_saut = 0;


        SDL_Event event;



while(jouer){


        //MENU
            if(lectureBestScore() != meilleureScore)
            {
            meilleureScore = lectureBestScore();
            }

    sprintf(texteBestScore, "BestScore : %d", meilleureScore);

    bestScore = TTF_RenderText_Blended(police, texteBestScore, couleurBlanche);


    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    SDL_BlitSurface(background, NULL, ecran, &positionBackground);
    SDL_BlitSurface(backgroundFloor, NULL, ecran, &positionBackgroundFloor);
    SDL_BlitSurface(texteJouer, NULL, ecran, &positionTexteJouer);
    SDL_BlitSurface(texteNom, NULL, ecran, &positionTexteNom);
    SDL_BlitSurface(bestScore, NULL, ecran, &positionBestScore);
    SDL_Flip(ecran);


    caisse1 = IMG_Load("img/caisse.jpg");
    caisse2 = IMG_Load("img/caisse.jpg");


    scoreNombre = 0;
    sprintf(scoreChar, "Score : %d", scoreNombre);
    score = TTF_RenderText_Blended(police, scoreChar, couleurBlanche);



        SDL_Event menu;

        while(SDL_PollEvent(&menu))
    {

        switch(menu.type)
        {

        case SDL_QUIT:


            continuer = 0;
            jouer = 0;

            break;

        case SDL_MOUSEBUTTONUP:

            if(menu.button.button == SDL_BUTTON_LEFT && menu.button.x >= 640 / 3 && menu.button.x <= 427 && menu.button.y >= 480 / 2 && menu.button.y <=380)
            {
                Mix_PlayMusic(musiqueBackground, -1);

                nombreAlleatoire = (rand() % (MAX -MIN + 1)) + MIN; //nombre alleatoire pour position X de la caisse
                nombreAlleatoire2 = (rand() % (MAX2 -MIN2 + 1)) + MIN2; //nombre alleatoire pour position x de l'objectif
                nombreAlleatoire3 = (rand() % (MAX - MIN + 1)) + MIN; //nombre alleatoire pour position x de la caisse 2

                if(nombreAlleatoire3 >= nombreAlleatoire + 34 && nombreAlleatoire3 <= nombreAlleatoire + (6 * 34) && nombreAlleatoire3 <= nombreAlleatoire + 34)
                {
                    nombreAlleatoire3 = nombreAlleatoire + (7 * 34);
                }

                positionCaisse.x = nombreAlleatoire;
                positionCaisse.y = 447;

                positionObjectif.x = nombreAlleatoire2;
                positionObjectif.y = 446;

                positionCaisse2.x = nombreAlleatoire3;
                positionCaisse2.y = 447;

                positionMario.y = 395;

                img_saut = 0;

                continuer = 1;

            }


            break;
        }
    }



    while(continuer)

    {

        if(img_saut != 0)
        {

            switch(img_saut % 10)
            {

        case 0:
            Mario = IMG_Load("img/jump1.png");
            img_saut++;
            break;
        case 1:
            Mario = IMG_Load("img/jump1.png");
            img_saut++;
            positionMario.y -= 40;
            break;
        case 2:
            Mario = IMG_Load("img/jump2.png");
            img_saut++;
            positionMario.y -= 40;
            break;
        case 3:
            Mario = IMG_Load("img/jump2.png");
            img_saut++;
            break;
        case 4:
            Mario = IMG_Load("img/jump3.png");
            img_saut++;
            break;
        case 5:
            Mario = IMG_Load("img/jump3.png");
            img_saut++;
            break;
        case 6:
            Mario = IMG_Load("img/jump3.png");
            img_saut++;
            break;
        case 7:
            Mario = IMG_Load("img/jump3.png");
            img_saut++;
            break;
        case 8:
            Mario = IMG_Load("img/jump4.png");
            img_saut++;
            positionMario.y += 40;
            break;
        case 9:
            Mario = IMG_Load("img/jump1.png");
            img_saut = 0;
            positionMario.y += 40;
            break;
            }

        }


        if(img_saut == 0)
    {

        switch(img_joueur % 8)
        {

    case 0:
        Mario = IMG_Load("img/mario1.png");
        break;
    case 1:
        Mario = IMG_Load("img/mario2.png");
        break;
    case 2:
        Mario = IMG_Load("img/mario3.png");
        break;
    case 3:
        Mario = IMG_Load("img/mario4.png");
        break;
    case 4:
        Mario = IMG_Load("img/mario5.png");
        break;
    case 5:
        Mario = IMG_Load("img/mario6.png");
        break;
    case 6:
        Mario = IMG_Load("img/mario7.png");
        break;
    case 7:
        Mario = IMG_Load("img/mario8.png");
        break;

        }

    }


    int resultatImgFloor = img_floor % 2;

    if(resultatImgFloor == 0)
    {
        positionBackgroundFloor.x = 0;

    } else if(resultatImgFloor == 1)

    {
        positionBackgroundFloor.x  = 20;
    }




        switch(img_cloud % 10)
        {

        case 0:
            backgroundCloud = IMG_Load("img/backgroundCloud.png");
            break;
        case 1:
            backgroundCloud = IMG_Load("img/backgroundCloud2.png");
            break;
        case 2:
            backgroundCloud = IMG_Load("img/backgroundCloud3.png");
            break;
        case 3:
            backgroundCloud = IMG_Load("img/backgroundCloud4.png");
            break;
        case 4:
            backgroundCloud = IMG_Load("img/backgroundCloud5.png");
            break;
        case 5:
            backgroundCloud = IMG_Load("img/backgroundCloud6.png");
            break;
        case 6:
            backgroundCloud = IMG_Load("img/backgroundCloud7.png");
            break;
        case 7:
            backgroundCloud = IMG_Load("img/backgroundCloud8.png");
            break;
        case 8:
            backgroundCloud = IMG_Load("img/backgroundCloud9.png");
            break;
        case 9:
            backgroundCloud = IMG_Load("img/backgroundCloud10.png");
            break;
        }


    SDL_Delay(50);




    nombreAlleatoire = (rand() % (MAX -MIN + 1)) + MIN; //nombre alleatoire pour position X de la caisse
    nombreAlleatoire2 = (rand() % (MAX2 -MIN2 + 1)) + MIN2; //nombre alleatoire pour position x de l'objectif
    nombreAlleatoire3 = (rand() % (MAX - MIN + 1)) + MIN; //nombre alleatoire pour position x de la caisse 2

    if(nombreAlleatoire3 <= nombreAlleatoire + (6 * 34)) //nombreAlleatoire3 >= nombreAlleatoire + 34 && nombreAlleatoire3 <= nombreAlleatoire + (6 * 34) && nombreAlleatoire3 <= nombreAlleatoire + 34
    {
        nombreAlleatoire3 = nombreAlleatoire + (7 * 34);
    }

    SDL_BlitSurface(background, NULL, ecran, &positionBackground);
    SDL_BlitSurface(backgroundFloor, NULL, ecran, &positionBackgroundFloor);
    SDL_BlitSurface(backgroundCloud, NULL, ecran, &positionBackgroundCloud);
    SDL_BlitSurface(Mario, NULL, ecran, &positionMario);
    SDL_BlitSurface(caisse1, NULL, ecran, &positionCaisse);
    SDL_BlitSurface(score, NULL, ecran, &positionScore);
    SDL_BlitSurface(bestScore, NULL, ecran, &positionBestScore);
    SDL_BlitSurface(objectif, NULL, ecran, &positionObjectif);
    SDL_BlitSurface(caisse2, NULL, ecran, &positionCaisse2);

    SDL_Flip(ecran);



    while(SDL_PollEvent(&event))
    {

    switch(event.type)
    {
        case SDL_QUIT:



            continuer = 0;
            jouer = 0;
            break;
    }

         if (event.type == SDL_KEYDOWN)
         {
            SDLKey keyPressed = event.key.keysym.sym;



            switch (keyPressed)
            {
               case SDLK_SPACE:
                  if(positionMario.y >= 390 )
                        {
                    Mix_PlayChannel(3, musiqueSaut, 0);
                    img_saut++;
                    //positionMario.y -= 50;
                    //gravite = 1;
                        }
                  break;
            }
         }

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(background, NULL, ecran, &positionBackground);
        SDL_BlitSurface(backgroundFloor, NULL, ecran, &positionBackgroundFloor);
        SDL_BlitSurface(backgroundCloud, NULL, ecran, &positionBackgroundCloud);
        SDL_BlitSurface(Mario, NULL, ecran, &positionMario);
        SDL_BlitSurface(caisse1, NULL, ecran, &positionCaisse);
        SDL_BlitSurface(score, NULL, ecran, &positionScore);
        SDL_BlitSurface(bestScore, NULL, ecran, &positionBestScore);
        SDL_BlitSurface(objectif, NULL, ecran, &positionObjectif);
        SDL_BlitSurface(caisse2, NULL, ecran, &positionCaisse2);

                    SDL_Flip(ecran);


        /*switch(event.type)
    {
        case SDL_QUIT:



            continuer = 0;
            jouer = 0;
            break;

        case SDL_KEYDOWN:
             switch(event.key.keysym.sym)
                {
                    case SDLK_UP:
                        if(positionMario.y == 395 )
                        {
                    Mix_PlayChannel(3, musiqueSaut, 0);
                    positionMario.y -= 50;
                    gravite = 1;

                    SDL_Flip(ecran);
                        }

                    break;
/*
                    default:
                        break;
                }

            break;
                    default:
                        break;

    } */
    }


    //GRAVITE

   /* if(gravite != 0)
        {
        tempsActuel = SDL_GetTicks();

            if(tempsActuel - ancienTemps >= temps)
                {
                    ancienTemps = tempsActuel;
                    positionMario.y += 50;
                    gravite = 0;
                }

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(background, NULL, ecran, &positionBackground);
        SDL_BlitSurface(backgroundFloor, NULL, ecran, &positionBackgroundFloor);
        SDL_BlitSurface(backgroundCloud, NULL, ecran, &positionBackgroundCloud);
        SDL_BlitSurface(Mario, NULL, ecran, &positionMario);
        SDL_BlitSurface(caisse1, NULL, ecran, &positionCaisse);
        SDL_BlitSurface(score, NULL, ecran, &positionScore);
        SDL_BlitSurface(bestScore, NULL, ecran, &positionBestScore);
        SDL_BlitSurface(objectif, NULL, ecran, &positionObjectif);
        SDL_BlitSurface(caisse2, NULL, ecran, &positionCaisse2);
        SDL_Flip(ecran);
    } */



        //CAISSE ARRIVEE A LA FIN


    if(positionCaisse.x <= 1)
    {
        positionCaisse.x = nombreAlleatoire;

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(background, NULL, ecran, &positionBackground);
        SDL_BlitSurface(backgroundFloor, NULL, ecran, &positionBackgroundFloor);
        SDL_BlitSurface(backgroundCloud, NULL, ecran, &positionBackgroundCloud);
        SDL_BlitSurface(Mario, NULL, ecran, &positionMario);
        SDL_BlitSurface(caisse1, NULL, ecran, &positionCaisse);
        SDL_BlitSurface(score, NULL, ecran, &positionScore);
        SDL_BlitSurface(bestScore, NULL, ecran, &positionBestScore);
        SDL_BlitSurface(objectif, NULL, ecran, &positionObjectif);
        SDL_BlitSurface(caisse2, NULL, ecran, &positionCaisse2);

        SDL_Flip(ecran);

        scoreNombre += 1;
        sprintf(scoreChar, "Score : %d", scoreNombre);
        score = TTF_RenderText_Blended(police, scoreChar, couleurBlanche);

            nombreAlleatoire = (rand() % (MAX -MIN + 1)) + MIN; //nombre alleatoire pour position X de la caisse
            nombreAlleatoire2 = (rand() % (MAX2 -MIN2 + 1)) + MIN2; //nombre alleatoire pour position x de l'objectif
            nombreAlleatoire3 = (rand() % (MAX - MIN + 1)) + MIN; //nombre alleatoire pour position x de la caisse 2

            if(nombreAlleatoire3 <= nombreAlleatoire + (6 * 34) )
            {
                nombreAlleatoire3 = nombreAlleatoire + (7 * 34);
            }


    }

            //CAISSE 2 ARRIVEE A LA FIN

    if(positionCaisse2.x <= 1)
    {
        positionCaisse2.x = nombreAlleatoire3;

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(background, NULL, ecran, &positionBackground);
        SDL_BlitSurface(backgroundFloor, NULL, ecran, &positionBackgroundFloor);
        SDL_BlitSurface(backgroundCloud, NULL, ecran, &positionBackgroundCloud);
        SDL_BlitSurface(Mario, NULL, ecran, &positionMario);
        SDL_BlitSurface(caisse1, NULL, ecran, &positionCaisse);
        SDL_BlitSurface(score, NULL, ecran, &positionScore);
        SDL_BlitSurface(bestScore, NULL, ecran, &positionBestScore);
        SDL_BlitSurface(objectif, NULL, ecran, &positionObjectif);
        SDL_BlitSurface(caisse2, NULL, ecran, &positionCaisse2);

        SDL_Flip(ecran);

        scoreNombre += 1;
        sprintf(scoreChar, "Score : %d", scoreNombre);
        score = TTF_RenderText_Blended(police, scoreChar, couleurBlanche);

            nombreAlleatoire = (rand() % (MAX -MIN + 1)) + MIN; //nombre alleatoire pour position X de la caisse
            nombreAlleatoire2 = (rand() % (MAX2 -MIN2 + 1)) + MIN2; //nombre alleatoire pour position x de l'objectif
            nombreAlleatoire3 = (rand() % (MAX - MIN + 1)) + MIN; //nombre alleatoire pour position x de la caisse 2

            if(nombreAlleatoire3 <= nombreAlleatoire + (6 * 34) )
            {
                nombreAlleatoire3 = nombreAlleatoire + (7 * 34);
            }



    }

            //OBJECTIF ARRIVE A LA FIN


        if(positionObjectif.x <= 1)
        {
            positionObjectif.x = 1200;

            SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
            SDL_BlitSurface(background, NULL, ecran, &positionBackground);
            SDL_BlitSurface(backgroundFloor, NULL, ecran, &positionBackgroundFloor);
            SDL_BlitSurface(backgroundCloud, NULL, ecran, &positionBackgroundCloud);
            SDL_BlitSurface(Mario, NULL, ecran, &positionMario);
            SDL_BlitSurface(caisse1, NULL, ecran, &positionCaisse);
            SDL_BlitSurface(score, NULL, ecran, &positionScore);
            SDL_BlitSurface(bestScore, NULL, ecran, &positionBestScore);
            SDL_BlitSurface(objectif, NULL, ecran, &positionObjectif);
            SDL_BlitSurface(caisse2, NULL, ecran, &positionCaisse2);

            SDL_Flip(ecran);

        }




            //COLLISION CAISSE


   if((colision(positionMario, positionCaisse) == 1))
    {
        Mix_PlayChannel(4, musiqueBoum, 0);
        Mix_PlayChannel(2, musiqueGameOver, 0);
        Mix_PauseMusic();
        gameOverChar[10] = "Game Over";
        gameOver = TTF_RenderText_Blended(policeGameOver, gameOverChar, couleurBlanche);


        continuer = 0;
        SDL_BlitSurface(gameOver, NULL, ecran, &positionGameOver);
        SDL_Flip(ecran);


        if(meilleureScore < scoreNombre)
                    {
        ecrireScore(scoreNombre);
                    }

        caisse1 = IMG_Load("img/caisse2.jpg");

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(background, NULL, ecran, &positionBackground);
        SDL_BlitSurface(backgroundCloud, NULL, ecran, &positionBackgroundCloud);
        SDL_BlitSurface(Mario, NULL, ecran, &positionMario);
        SDL_BlitSurface(caisse1, NULL, ecran, &positionCaisse);
        SDL_BlitSurface(score, NULL, ecran, &positionScore);
        SDL_BlitSurface(bestScore, NULL, ecran, &positionBestScore);
        SDL_BlitSurface(objectif, NULL, ecran, &positionObjectif);
        SDL_BlitSurface(caisse2, NULL, ecran, &positionCaisse2);
        SDL_BlitSurface(gameOver, NULL, ecran, &positionGameOver);
        SDL_Flip(ecran);

        SDL_Delay(2000);

        positionCaisse.x = nombreAlleatoire;
        positionObjectif.x = nombreAlleatoire2;
        positionCaisse2.x = nombreAlleatoire3;



    }

       if((colision(positionMario, positionCaisse2) == 1) )
    {
        Mix_PlayChannel(4, musiqueBoum, 0);
        Mix_PlayChannel(2, musiqueGameOver, 0);
        Mix_PauseMusic();
        gameOverChar[10] = "Game Over";
        gameOver = TTF_RenderText_Blended(policeGameOver, gameOverChar, couleurBlanche);


        continuer = 0;
        SDL_BlitSurface(gameOver, NULL, ecran, &positionGameOver);
        SDL_Flip(ecran);

        if(meilleureScore < scoreNombre)
                    {
        ecrireScore(scoreNombre);
                    }

        caisse2 = IMG_Load("img/caisse2.jpg");

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(background, NULL, ecran, &positionBackground);
        SDL_BlitSurface(backgroundCloud, NULL, ecran, &positionBackgroundCloud);
        SDL_BlitSurface(Mario, NULL, ecran, &positionMario);
        SDL_BlitSurface(caisse1, NULL, ecran, &positionCaisse);
        SDL_BlitSurface(score, NULL, ecran, &positionScore);
        SDL_BlitSurface(bestScore, NULL, ecran, &positionBestScore);
        SDL_BlitSurface(objectif, NULL, ecran, &positionObjectif);
        SDL_BlitSurface(caisse2, NULL, ecran, &positionCaisse2);
        SDL_BlitSurface(gameOver, NULL, ecran, &positionGameOver);
        SDL_Flip(ecran);

        SDL_Delay(2000);

        positionCaisse.x = nombreAlleatoire;
        positionObjectif.x = nombreAlleatoire2;
        positionCaisse2.x = nombreAlleatoire3;



    }





        if(colision(positionMario, positionObjectif) == 1)
        {

            scoreNombre++;
            Mix_PlayChannel(1, musiquePiece, 0);

            SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
            SDL_BlitSurface(background, NULL, ecran, &positionBackground);
            SDL_BlitSurface(backgroundFloor, NULL, ecran, &positionBackgroundFloor);
            SDL_BlitSurface(backgroundCloud, NULL, ecran, &positionBackgroundCloud);
            SDL_BlitSurface(caisse1, NULL, ecran, &positionCaisse);
            SDL_BlitSurface(score, NULL, ecran, &positionScore);
            SDL_BlitSurface(bestScore, NULL, ecran, &positionBestScore);
            SDL_BlitSurface(objectif, NULL, ecran, &positionObjectif);
            SDL_BlitSurface(caisse2, NULL, ecran, &positionCaisse2);
            SDL_Flip(ecran);

                positionObjectif.x = 1200;

            SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
            SDL_BlitSurface(background, NULL, ecran, &positionBackground);
            SDL_BlitSurface(backgroundFloor, NULL, ecran, &positionBackgroundFloor);
            SDL_BlitSurface(backgroundCloud, NULL, ecran, &positionBackgroundCloud);
            SDL_BlitSurface(Mario, NULL, ecran, &positionMario);
            SDL_BlitSurface(caisse1, NULL, ecran, &positionCaisse);
            SDL_BlitSurface(score, NULL, ecran, &positionScore);
            SDL_BlitSurface(objectif, NULL, ecran, &positionObjectif);
            SDL_BlitSurface(caisse2, NULL, ecran, &positionCaisse2);
            SDL_Flip(ecran);

            sprintf(scoreChar, "Score : %d", scoreNombre);
            score = TTF_RenderText_Blended(police, scoreChar, couleurBlanche);

            }


            //FIN COLLISION OBJECTIF


        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(background, NULL, ecran, &positionBackground);
        SDL_BlitSurface(backgroundFloor, NULL, ecran, &positionBackgroundFloor);
        SDL_BlitSurface(backgroundCloud, NULL, ecran, &positionBackgroundCloud);
        SDL_BlitSurface(Mario, NULL, ecran, &positionMario);
        SDL_BlitSurface(caisse1, NULL, ecran, &positionCaisse);
        SDL_BlitSurface(score, NULL, ecran, &positionScore);
        SDL_BlitSurface(bestScore, NULL, ecran, &positionBestScore);
        SDL_BlitSurface(objectif, NULL, ecran, &positionObjectif);
        SDL_BlitSurface(caisse2, NULL, ecran, &positionCaisse2);
        SDL_Flip(ecran);


        img_joueur++;
        img_floor++;
        img_cloud++;
    }

}


    Mix_FreeMusic(musiqueBackground);
    TTF_CloseFont(police);

    SDL_FreeSurface(Mario);


    fclose(fichier);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}

Uint32 avancerCaisse(Uint32 intervalle, void *parametre)
{

    SDL_Rect* positionCaisse = parametre; /* Conversion de void* en SDL_Rect* */
    positionCaisse->x-=10;
return intervalle;
}

Uint32 avancerObjectif(Uint32 intervalle, void *parametre)
{

    SDL_Rect* positionObjectif = parametre; /* Conversion de void* en SDL_Rect* */
    positionObjectif->x-=10;
return intervalle;
}

Uint32 avancerCaisse2(Uint32 intervalle, void *parametre)
{

    SDL_Rect* positionCaisse = parametre; /* Conversion de void* en SDL_Rect* */
    positionCaisse->x-=10;
return intervalle;
}

    int colision(SDL_Rect positionJoueur, SDL_Rect positionObstacle)
    {

    if((positionJoueur.x + 70 == positionObstacle.x && positionJoueur.y + 95  <= 442) || (positionJoueur.y + 90 >= positionObstacle.y && positionJoueur.x + 70 >= positionObstacle.x && positionJoueur.x <= positionObstacle.x + 34))
    {

        return 1;

    }

    return 0;

    }



    int colisionObjectif(SDL_Rect positionJoueur, SDL_Rect positionObjectif)
    {

    if((positionJoueur.x + 70 == positionObjectif.x && positionJoueur.y >= 446) || (positionJoueur.y + 90 >= positionObjectif.y && positionJoueur.x + 70 >= positionObjectif.x && positionJoueur.x <= positionObjectif.x + 34))
    {

        return 1;

    }

    return 0;

    }

    void ecrireScore(int scoreNombre)
    {

        FILE*  fichier = NULL;
        char BestScoreEnregistre[10];

                    fichier = fopen("score.txt", "w+");

                    sprintf(BestScoreEnregistre, "%d", scoreNombre);
                    fputs(BestScoreEnregistre, fichier);

                    fclose(fichier);


        }



    int lectureBestScore(int bestScore)
    {



        FILE*  fichier = NULL;

        char precedentBestScore[3];

        fichier = fopen("score.txt", "r");

        fgets(precedentBestScore, 100, fichier);
        int meilleureScore = atoi(precedentBestScore);


    fclose(fichier);

    return meilleureScore;


        }







