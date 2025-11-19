#ifndef ENTITIES_H
#define ENTITIES_H

typedef struct {
    int id;
    char name[50];
} Genre;

typedef struct {
    int id;
    int winCount;
    int nominationCount;
} Stat;

typedef struct {
    int id;
    int *genreId;
    int *titleId;
} GenreTitle;

typedef struct {
    int id;
    char description[100];
    int aggregateRating;
    int voteCount;
} Rating;

typedef struct {
    int id;
    int *statId;
    int *ratingId;
    char type[50];
    int isAdult;
    char primaryTitle[100];
    int startYear;
    int endYear;
} Title;

typedef struct {
    int id;
    int *titleId;
    int *ratingId;
    char title[100];
    int season;
    int episodeNumber;
} Episode;

typedef struct {
    int id;
    char name[50];
} Event;

typedef struct {
    int id;
    char category[50];
    char text[100];
    int year;
} Award;

typedef struct {
    int id;
    int *eventId;
    int *awardId;
} EventAward;

typedef struct {
    int id;
    int *awardId;
    int *titleId;
    int isWinner;
} Nomination;

#endif //ENTITIES_H
