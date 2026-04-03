#include "simple_logger.h"
#include "ent.h"
#include "projectile.h"
#include "prop.h"
#include "player.h"
#include "camera.h"

    /**
    * Z-index
    *
    * Agnostic : Background / Parallax Objects
    *
    *  1 - 3 : World Objects
    *  4 : Items/Consumables
    *
    *  6 : Low Particles
    *  6 : Inactive NPC
    *  7 : Inactive player
    *  8 : Active NPC
    *  9 : Active Player
    *  10 : High Particles
    *
    * Moving player will show above a moving npc
    * Moving npcs will show above a moving player
    * Idle player will show above an idle npc
    *
    *  11: Covering Object
    *
    **/



    static eCOUNT entCount = {0};
    static EntManager entManager = {0};
    static Ent **worldObjects  = {0};
    static Ent **dynamicObjects = {0};
    static Ent **playerObjects = {0};


    void ent_hit(Ent *attacker, Ent *self, Uint8 tag) {

        if(self->hit)self->hit(attacker, self, tag); // easiest way to do a on hit

    }

    void ent_score(Uint8 amount, Uint8 team) {

        if (team == 1) {
            entManager.level->game.t1_points += amount;
        } else if(team == 2) {
            entManager.level->game.t2_points += amount;
        }

    }

    void insert_collision_layer(Ent *self){

        switch (self->_tags){

            case TAG_PLAYER:
                dynamicObjects[entCount.oc_dynamic] = self;
                playerObjects[entCount.oc_player] = self;
                entCount.oc_dynamic++;
                entCount.oc_player++;
                break;

            case TAG_NPC:
                dynamicObjects[entCount.oc_dynamic] = self;
                playerObjects[entCount.oc_player] = self;
                entCount.oc_dynamic++;
                entCount.oc_player++;
                break;

            case TAG_DYNAMIC:
                dynamicObjects[entCount.oc_dynamic] = self;
                entCount.oc_dynamic++;
                entCount.oc_dprop++;
                break;

            case TAG_STATIC:
                worldObjects[entCount.oc_world] = self;
                entCount.oc_world++;
                entCount.oc_sprop++;
                break;

            case TAG_CONSUMABLE:
                worldObjects[entCount.oc_world] = self;
                entCount.oc_world++;
                entCount.oc_item++;
                break;
            case TAG_PROJECTILE:
                dynamicObjects[entCount.oc_dynamic] = self;
                entCount.oc_dynamic++;
                entCount.oc_projectile++;
            default:

                break;
        }
        entCount.oc_all++;

    }


    void ent_collide_all(){


        int i;

        for (i = 0; i <  entCount.oc_dynamic; i++){

            if (dynamicObjects[i]->_inuse && dynamicObjects[i]->_tags > 0){
        // slog("entity number %d can collide", i);
            ent_collide((Ent*)dynamicObjects[i]);
        }

    }


    }

    void ent_collide(Ent *self){

        // World collide first

        int i;

        self->collide.c_color = GFC_COLOR_RED;
        // set grounded to zero
        self->status &= ~1;

        if(!self)return;

        for (i = 0; i <  entCount.oc_world; i++){

            if(worldObjects[i] == self)continue;

            if (gfc_rect_overlap(self->collide.c_box, worldObjects[i]->collide.c_box)){

            self->collide.c_color = GFC_COLOR_GREEN;
            worldObjects[i]->collide.c_color = GFC_COLOR_GREEN;

            if (worldObjects[i]->collide.c_mask == CM_TRIGGER){
                prop_trigger(self, worldObjects[i], entManager.level );
                continue;
;            }

            if(worldObjects[i]->collide.c_mask == CM_BLOCKER || worldObjects[i]->collide.c_mask == CM_PASSTHROUGH){
                if (self->_tags == TAG_PROJECTILE){
                                    projectile_hit(self, worldObjects[i]);
                                }
                double s_x1 = self->collide.c_box.x, s_y1 = self->collide.c_box.y;
                double w_x1 = worldObjects[i]->collide.c_box.x, w_y1 = worldObjects[i]->collide.c_box.y;
                double s_x2 = s_x1 + self->collide.c_box.w, s_y2 = s_y1 + self->collide.c_box.h;
                double w_x2 = w_x1 + worldObjects[i]->collide.c_box.w, w_y2 = w_y1 + worldObjects[i]->collide.c_box.h;
            // Calculate origin distance
            // use origin distance to gauge which has the lowest

                double left_overlap = s_x2 - w_x1;
                double right_overlap = w_x2 - s_x1;
                double top_overlap = s_y2 - w_y1;
                double bot_overlap = w_y2 - s_y1;
                double min_pen = top_overlap;

                min_pen = MIN(min_pen, bot_overlap);
                min_pen = MIN(min_pen, left_overlap);
                min_pen = MIN(min_pen, right_overlap);

                if (worldObjects[i]->collide.c_mask == CM_PASSTHROUGH) {
                    if ((self->status & 34)!= 0 || (s_y2 > w_y2 && s_y1 < w_y2)) {
                    continue;
                    } else {
                    if (min_pen == bot_overlap) {
                        self->transform.position.y += bot_overlap;
                    }
                    }

                }

                if (min_pen == top_overlap){
                    self->status = (self->status | 1);
                    self->transform.position.y -= top_overlap;

                }
                else if (min_pen  == left_overlap) {
                    self->transform.position.x -= left_overlap;
                }
                else if (min_pen == right_overlap) {
                    self->transform.position.x += right_overlap;
                } else if (min_pen == bot_overlap) {
                    self->transform.position.y += bot_overlap;

                }

                }
            }
        }


        // dynamic collide next

        for (i = 0; i <  entCount.oc_dynamic; i++){

            if(dynamicObjects[i] == self)continue;

            if (gfc_rect_overlap(self->collide.c_box, dynamicObjects[i]->collide.c_box)){
                if (self->_tags == TAG_PROJECTILE && dynamicObjects[i]->_tags != TAG_PROJECTILE){
                    ent_hit(self, dynamicObjects[i], self->_tags);
                    projectile_hit(self, dynamicObjects[i]);
                }
                self->collide.c_color = GFC_COLOR_GREEN;
                dynamicObjects[i]->collide.c_color = GFC_COLOR_GREEN;

            }

        }

    }


    void link_level(Level *l){

        entManager.level = l;

    }

    void ent_manager_init(Uint32 max) {

        if(!max){
            slog("cannot initialize entity system with zero entities");
            return;
        }
        entManager.entList = gfc_allocate_array(sizeof(Ent), max);
        worldObjects = gfc_allocate_array(sizeof(Ent *), max);
        dynamicObjects = gfc_allocate_array(sizeof(Ent *), max);
        playerObjects = gfc_allocate_array(sizeof(Ent *), 16);

        if(!entManager.entList){
            slog("failed to allocate %i entities", max);
            return;
        }
        entManager.entMax = max;

        atexit(ent_manager_close);
        slog("initialized entity System.");
    }

    void ent_manager_close(){
    int i;
    if (!entManager.entList)return;
        for (i = 0; i < entManager.entMax; i++){
            ent_free(&entManager.entList[i]);
        }

        free(entManager.entList);
        free(dynamicObjects);
        free(worldObjects);
        free(playerObjects);
        memset(&entManager, 0, sizeof(EntManager));
        memset(&worldObjects, 0, sizeof(worldObjects));
        memset(&playerObjects, 0, sizeof(playerObjects));
        memset(&dynamicObjects, 0, sizeof(dynamicObjects));


        slog("closed entity system");

    }

    /**
    * @brief Ent Create / Destroy
    **/

    Ent *ent_new(){
        int i;
        if (!entManager.entList){
            slog("eMan not initialized");
            return NULL;
        }
        for (i = 0; i < entManager.entMax; i++){
            if(entManager.entList[i]._inuse) continue;
            entManager.entList[i]._inuse = 1;

            // set defaults
            entManager.entList[i].transform.position = gfc_vector2d(0, 0);
            entManager.entList[i].transform.scale = gfc_vector2d(1, 1);
            entManager.entList[i].transform.center = gfc_vector2d(0,0);
            entManager.entList[i].transform.velocity = gfc_vector2d(0,0);
            entManager.entList[i].status = 0;

            entManager.entList[i].collide.c_dim = gfc_vector2d(1,1);
            gfc_rect_set(entManager.entList[i].collide.c_box, 0, 0, 1, 1);
            entManager.entList[i].collide.c_color = GFC_COLOR_RED;
            entManager.entList[i].collide.c_mask = CM_NONE;


            return &entManager.entList[i];
        }
        return NULL;
    }

    // adds entities to back of the list, for props and misc sprites and such

    Ent *ent_new_rev(){
        int i;
        if (!entManager.entList){
            slog("eMan not initialized");
            return NULL;
        }
        for (i = entManager.entMax-1; i > 0; i--){

            if(entManager.entList[i]._inuse) continue;
            entManager.entList[i]._inuse = 1;
            entManager.entList[i].transform.position = gfc_vector2d(0, 0);
            entManager.entList[i].transform.scale = gfc_vector2d(1, 1);
            entManager.entList[i].transform.velocity = gfc_vector2d(0,0);
            entManager.entList[i].status = 0;
            // set default


            entManager.entList[i].collide.c_dim = gfc_vector2d(1,1);
            gfc_rect_set(entManager.entList[i].collide.c_box, 0, 0, 1, 1);
            entManager.entList[i].collide.c_color = GFC_COLOR_RED;
            entManager.entList[i].collide.c_mask = CM_NONE;
            return &entManager.entList[i];
        }
        return NULL;
    }


    void ent_free(Ent *self){
        if(!self) return;
        if(self->sprite)gf2d_sprite_free(self->sprite);
        if(self->free)self->free(self); // individual free
        switch (self->_tags){

            case TAG_PLAYER:
                entCount.oc_dynamic--;
                entCount.oc_player--;
                break;

            case TAG_NPC:
                entCount.oc_dynamic--;
                entCount.oc_player--;
                break;

            case TAG_DYNAMIC:
                entCount.oc_dynamic--;
                entCount.oc_dprop--;
                break;

            case TAG_STATIC:
                entCount.oc_world--;
                entCount.oc_sprop--;
                break;

            case TAG_CONSUMABLE:
                entCount.oc_world--;
                entCount.oc_item--;
                break;
            case TAG_PROJECTILE:
                entCount.oc_dynamic--;
                entCount.oc_projectile--;
            default:

                break;
        }

        if(self->stats) free(self->stats);
        entCount.oc_all--;
        memset(self, 0, sizeof(Ent));
    }


    void ent_clear()    {
        int i;
        if (!entManager.entList){
            slog("Entity Manager not initialized");
            return;
        }
        for (i = 0; i < entManager.entMax; i++){
            if(!(entManager.entList[i]._inuse)) continue;
            ent_free(&entManager.entList[i]);
        }

        memset(&entCount, 0, sizeof(entCount));

    }
    /**
    * @brief Ent Draw Functions
    **/

    void ent_draw(Ent *self){

        if (!self)return;

        if(self->stats){

        if (self->stats[0] <= 0) {
            return;
        }

        }
        int xlength = self->collide.c_dim.x;
        int ylength = self->collide.c_dim.y;


        if(!self)return;
        GFC_Vector2D offset, position;
        offset = camera_get_pos();

        gfc_vector2d_sub(position, self->transform.position, offset);

        // base
        if(self->sprite){
        gf2d_sprite_render(
            self->sprite,
            position,
            &self->transform.scale,
            &self->transform.center,
            &self->transform.rotation,
            NULL,
            &self->color,
            NULL,
            (Uint32) self->frame);
        }


        gfc_rect_set(self->collide.c_box,position.x,position.y,xlength*10,ylength*10);
        GFC_Rect hitbox = gfc_rect(position.x,position.y,xlength*10,ylength*10);
        gf2d_draw_rect(hitbox, self->collide.c_color);

        if(self->draw)self->draw(self);
    }

    void ent_manager_draw_all(){
        int i;
        if (!entManager.entList){
            slog("Entity Manager not initialized");
            return;
        }
        for (i = 0; i < entManager.entMax; i++){
            if(entManager.entList[i]._inuse == 0) continue;
            ent_draw(&entManager.entList[i]);
        }
    }

    void ent_update(Ent *self, float deltatime){
        if(!self)return;

        gfc_rect_set(
            self->collide.c_box,
            self->transform.position.x,
            self->transform.position.y,
            self->collide.c_dim.x*10,
            self->collide.c_dim.y*10
        );

        // calls the player, prop, or npc behavior
        if(self->update)self->update(self, deltatime);
    }


    void ent_think(Ent *self){
        if(!self)return;

        if(self->think)self->think(self);
    }

    void ent_update_all(float deltatime){

        int i;
        for (i = 0; i < entManager.entMax; i++)  {
            if(!entManager.entList[i]._inuse)continue;
            ent_update(&entManager.entList[i], deltatime);
            if(entManager.entList[i]._tags == TAG_PLAYER || entManager.entList[i]._tags == TAG_NPC){
                if (entManager.level->game.max_points < get_points(&entManager.entList[i])) {
                    entManager.level->game.max_points = get_points(&entManager.entList[i]);
                }
            }
            entManager.entList[i].collide.c_color = GFC_COLOR_RED;
        }
        level_update(entManager.level);
        ent_collide_all();

    }

    void ent_think_all(){
        int i;
        for (i = 0; i < entManager.entMax; i++)  {
            if(!entManager.entList[i]._inuse)continue;
            ent_think(&entManager.entList[i]);
        }
    }


    Ent *get_player(){
    int i;
        for(i = 0; i < entCount.oc_player; i++ ){
            if (playerObjects[i]->_tags == TAG_PLAYER){
                return playerObjects[i];
            }
        }

        return NULL;
    }

    Ent *index_ent(int id){

        return &entManager.entList[id];

    }

