#include <pthread.h>
#include "IndianRail.h"


/**
  Initializes all the mutexes and condition-variables.
*/
void
station_init(struct station *station)
{
  station->out_passengers = 0;
  station->in_passengers = 0;
  pthread_mutex_init(&(station->lock), NULL);
  pthread_cond_init(&(station->train_arrived_cond), NULL);
  pthread_cond_init(&(station->passengers_seated_cond), NULL);
  pthread_cond_init(&(station->train_is_full_cond), NULL);
}

/**
Loads the train with passengers. When a passenger robot arrives in a station, it first invokes this function. 
The function must not return until the train is satisfactorily loaded.
Params:
  stattion: current station pointer
  count: indicates how many seats are available on the train
*/
void
station_load_train(struct station *station, int count)
{
  // Enter critical region
  pthread_mutex_lock(&(station->lock));

  while ((station->out_passengers > 0) && (count > 0)){
    pthread_cond_signal(&(station->train_arrived_cond));
  	count--;
  	pthread_cond_wait(&(station->passengers_seated_cond), &(station->lock));
  }

  if (station->in_passengers > 0)
  	pthread_cond_wait(&(station->train_is_full_cond), &(station->lock));

  // Leave critical region
  pthread_mutex_unlock(&(station->lock));
}

/**
This function must not return until a train is in the station and there are enough free seats on
the train for this passenger. Once this function returns, the passenger robot will move the
passenger on board the train and into a seat.
Once the passenger is seated, it will call the function: station_on_board
Params:
  stattion: current station pointe
*/
void
station_wait_for_train(struct station *station)
{
  pthread_mutex_lock(&(station->lock));

  station->out_passengers++;
  pthread_cond_wait(&(station->train_arrived_cond), &(station->lock));
  station->out_passengers--;
  station->in_passengers++;

  pthread_mutex_unlock(&(station->lock));

  pthread_cond_signal(&(station->passengers_seated_cond));
}

/**
Use this function to let the train know that it’s on board.
Params:
  stattion: current station pointer
*/
void
station_on_board(struct station *station)
{
  pthread_mutex_lock(&(station->lock));

  station->in_passengers--;

  pthread_mutex_unlock(&(station->lock));

  if (station->in_passengers == 0)
  	pthread_cond_broadcast(&(station->train_is_full_cond));
}
