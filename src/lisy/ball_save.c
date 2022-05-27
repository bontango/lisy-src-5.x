/*
 BALL_SAVE.c
 Mai 2022
 bontango
*/


void lisy_ball_save_init_event(void)
{
}

void lisy_ball_save_switch_event(int arg1, int arg2)
{
}

void lisy_ball_save_sol_event(int arg1)
{
}


//the Ball Save eventhandler
void lisy_ball_save_event_handler( int id, int arg1, int arg2)
{

    switch(id)
        {
         case LISY_HOME_SS_EVENT_INIT: lisy_ball_save_init_event( ); break;
         case LISY_HOME_SS_EVENT_SWITCH: lisy_ball_save_switch_event( arg1, arg2 ); break;
         case LISY_HOME_SS_EVENT_MOM_SOL: lisy_ball_save_sol_event( arg1 ); break;
        }

}

