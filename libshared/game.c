#include "game.h"

#include <stdlib.h>

_Bool p_id_predicate(void* item, void* data) {
    return ((player*)item)->id == *(int*)data;
}

void game_init(game* game) {
    pthread_mutex_init(&game->mutex, NULL);
    sll_init(&game->players, sizeof(player));
    game->ready_count = 0;
    game->started = 0;
    game->time = 0;
}

void add_player(game* game, player* p) {
    pthread_mutex_lock(&(game->mutex));
    sll_add(&game->players, p);
    pthread_mutex_unlock(&(game->mutex));
}

void remove_player(game* game, int player_id) {
    pthread_mutex_lock(&(game->mutex));
    size_t len = sll_get_size(&game->players);
    for (int i = 0; i < len; i++) {
        player* p = sll_get_ref(&game->players, i);
        if (p!= NULL && p->id == player_id) {
            sll_remove(&game->players, i);
        }
    }
    pthread_mutex_unlock(&(game->mutex));
}

void game_update_time(game* game, size_t new_time) {
    pthread_mutex_lock(&(game->mutex));
    game->time = new_time;
    pthread_mutex_unlock(&(game->mutex));
}

void game_update_score(game* game, int player_id, int new_score) {
    pthread_mutex_lock(&(game->mutex));
    player* p = sll_find(&game->players, p_id_predicate, &player_id);
    if (p != NULL) p->score = new_score;
    pthread_mutex_unlock(&(game->mutex));
}

void game_start(game* game, size_t time) {
    pthread_mutex_lock(&(game->mutex));
    game->time = time;
    game->started = 1;
    pthread_mutex_unlock(&(game->mutex));
}

void game_stop(game* game) {
    pthread_mutex_lock(&(game->mutex));
    game->started = 0;
    pthread_mutex_unlock(&(game->mutex));
}


void game_free(game* game) {
    pthread_mutex_destroy(&(game->mutex));
    sll_destroy(&game->players);
    free(game);
}

void score_clear(void* arg_p, void* context) {
    ((player*) arg_p)->score = 0;
}

void game_clear(game* game) {
    pthread_mutex_lock(&(game->mutex));
    game->started = 0;
    game->ready_count = 0;
    game->time = 0;
    sll_for_each(&game->players, score_clear, NULL);
    pthread_mutex_unlock(&(game->mutex));
}