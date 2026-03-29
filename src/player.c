#include "simple_logger.h"


#include "gfc_types.h"
#include "gfc_input.h"
#include "camera.h"
#include "world_def.h"
#include "player.h"

#define SPEED_CONSTANT 400.0f
#define JUMP_CONSTANT 800.0f

void player_free(Ent *self){
    PlayerData *pd = (PlayerData *) self->misc;

    if(pd->head) gf2d_sprite_free(pd->head);

}



void player_think(Ent *self){


    if(!self){
    return;
    }


    if(self->collide.c_mask == CM_TEAM1) {

        self->color = GFC_COLOR_BLUE;

    }
    if (self->collide.c_mask == CM_TEAM2) {

        self->color = GFC_COLOR_ORANGE;

    }
    if (self->collide.c_mask == CM_FFA) {

        self->color = GFC_COLOR_WHITE;

    }


    PlayerData *pd = (PlayerData *) self->misc;

    GFC_Vector2D offset = gfc_vector2d(self->transform.position.x + 25, self->transform.position.y-40);
    GFC_Vector2D mouse = get_mouse_pos();
    float dx = mouse.x - offset.x;
    float dy = mouse.y - offset.y;


    if (pd->hand->fdebounce <0 || pd->melee.debounce ){
        self->status ^= 16; // flips of attacking/shooting bit
    }
    if (pd->hand->rdebounce <0){
        self->status ^= 8; // flips of reloading bit
    }

if(self->_tags != TAG_PLAYER) return;

pd->hTransform.rotation = atan2f(dy,dx) * (180.0f / M_PI);

if(gfc_input_key_down("a")){
    move(self, LEFT);
}

if(gfc_input_key_down("d")){
    move(self, RIGHT);
}

if(gfc_input_key_down("s")){
    self->status |= 32;
} else {
    self->status &= ~(32U);
}

if(gfc_input_key_down(" ")){
    move(self, JUMP);
}

if(click_status()==1){

    if (self->status & 24) {

    fire(pd->hand, self);

    }

}


if(gfc_input_key_down("q") && pd->activeCharge >=100){
    activate_ability(self, pd->activeId);
    pd->activeCharge = 0;
}

if(gfc_input_key_down("1")){
    if((self->status & 24) == 0){
    pd->hand = pd->guns;
    }
}

if(gfc_input_key_down("2")){
    if((self->status & 24) == 0){
    pd->hand = pd->guns+1;
    }
}

if(gfc_input_key_down("3")){
    if((self->status & 24) == 0){
    pd->hand = pd->guns+2;
    }
}

if (gfc_input_key_down("f"))

    // attacking or melee
    if (self->status & 24) {
        attack(&(pd->melee));
    }

}

void move(Ent *self, int dir){
    if(!self)return;

    double direction = 0;

    if (dir == LEFT){
        direction += -1;
    }

    if (dir == RIGHT){
       direction += 1;
    }

    direction *= self->stats[SPEED];

    if (fabs(self->transform.velocity.x) < self->stats[SPEED]) {

       self->transform.velocity.x = direction;

    }

    if (dir == JUMP && ((self->status & 3) == 1)) {
        self->status |= 2;
         self->transform.velocity.y -= self->stats[JUMPPOWER];
         slog("jump");
    }

}


void player_update(Ent *self, float deltatime)
{
    if(!self)return;

    // check if airborne

    PlayerData *pd = (PlayerData *) self->misc;

    if (pd->melee.debounce > 0){
        pd->melee.debounce -= deltatime;
    }
    if (pd->hand->fdebounce > 0){
        pd->hand->fdebounce -= deltatime;
    }
    if (pd->hand->rdebounce > 0){
        pd->hand->rdebounce -= deltatime;
    }

    if ((self->status & 1) == 0){
        if ((self->status & 2) == 2){
        self->transform.velocity.y += JUMP_CONSTANT * (8.0f/5.0f) * deltatime;
        } else {
        self->transform.velocity.y += JUMP_CONSTANT * deltatime;
        }
    } else if ((self->status & 3) == 1){

        self->transform.velocity.y = 0;

    }
    if ((self->status & 2 ) == 2) {

        if(self->transform.velocity.y >= 0)
        {
            self->status -= 2;
        }
    }

    if (self->stats[ARMOR] >= 100){
        self->stats[ARMOR] = 100;
    }

    if (pd->passiveCd >= 1){
        apply_passive(self, pd->passiveId);
        pd->passiveCd = 0;
    } else {
        pd->passiveCd += deltatime;
    }

    if ((pd->activeCharge < 100)){
        pd->activeCharge += deltatime * 6;
    }

    if (pd->hillTimer > 0){
        pd->hillTimer -= deltatime;
    }

    self->transform.position.x += self->transform.velocity.x * deltatime;
    self->transform.position.y += self->transform.velocity.y * deltatime;
    pd->hTransform.position.x = self->transform.position.x+25;
    pd->hTransform.position.y = self->transform.position.y+10;
    // all gravity bound objects must fall


    if(fabs(self->transform.velocity.x) < GFC_EPSILON ){
        self->transform.velocity.x = 0;
    } else if ((self->status >> 2 & 4) == 0){
        // not walking so declerate
        if ((self->status & 1) == 1){
            self->transform.velocity.x *= 0.1;
        } else {
            self->transform.velocity.x *= 0.9;
        }
    }

    // check if dead
    if(self->stats[HEALTH] < 0) {

        if(self->status & 64 && self->collide.c_mask != CM_TEAM1){

            self->status &= ~(64U);

            if(self->collide.c_mask == CM_TEAM1){
                // respawn team1
            }

            if(self->collide.c_mask == CM_TEAM2){
                // respawn team2
            }
        }

        self->stats[RESPAWNTIMER] -= deltatime;
        if (self->stats[RESPAWNTIMER] <= 0 && self->_tags == TAG_PLAYER){
            player_respawn(self);
            return;
        }
    }

}


int get_points(Ent *self){

    PlayerData *pd = (PlayerData *) self->misc;

    return pd->points;

}

void player_draw(Ent *self){


        if(!self)return;

        if(self->stats[HEALTH] < 0) {
            return;
        }

        GFC_Vector2D offset, position;
        offset = camera_get_pos();

        PlayerData *pd = (PlayerData *) self->misc;

        // health bar

        float remaining_hp = 100.0f * (self->stats[HEALTH] / 1000.0f);
        float remaining_shield = 100.0f * (self->stats[ARMOR] / 100.0f);

        gfc_vector2d_sub(position, self->transform.position, offset);

        GFC_Rect health_bar = gfc_rect((position.x - 5), (position.y + 90 - (remaining_hp)), 5, remaining_hp);
        GFC_Rect armor_bar = gfc_rect((position.x - 15), (position.y + 90 - (remaining_shield)), 5, remaining_shield);

        gf2d_draw_rect_filled(health_bar, GFC_COLOR_RED);
        gf2d_draw_rect_filled(armor_bar, GFC_COLOR_GREY);

        gfc_vector2d_sub(position, pd->hTransform.position, offset);

        // base
        if(pd->head){
        gf2d_sprite_render(
            pd->head,
            position,
            &pd->hTransform.scale,
            &pd->hTransform.center,
            &pd->hTransform.rotation,
            NULL,
            &self->color,
            NULL,
            (Uint32) self->frame);
        }

        gfc_rect_set(pd->hCollide.c_box,position.x-25,position.y-25,pd->hCollide.c_dim.x*10,pd->hCollide.c_dim.x*10);
        gf2d_draw_rect(pd->hCollide.c_box, self->collide.c_color);

}


void player_respawn(Ent *self){

    Ent *player = player_new();
    PlayerData *pd1 = (PlayerData *) self->misc;
    PlayerData *pd2 = (PlayerData *) player->misc;
    *pd2 = *pd1;
    self->_tags = TAG_NONE;
    self->_inuse = 0;
    pd2->guns[0].reserve = pd2->guns[0].maxammo;
    pd2->guns[1].reserve = pd2->guns[1].maxammo;
    pd2->guns[2].reserve = pd2->guns[2].maxammo;
    pd2->guns[0].ammo = pd2->guns[0].apr;
    pd2->guns[1].ammo = pd2->guns[1].apr;
    pd2->guns[2].ammo = pd2->guns[2].apr;
    ent_free(self);
}

void player_hit (Ent *attacker, Ent *self, Uint8 tag){

    if (!attacker || !self) return;
    if (!attacker->misc) return;

    Projectile *p = (Projectile*) attacker->misc;

    if (p->hitCooldown > 0) return;

    if (!p) return;
    if (!p->owner) return;
    if (!p->owner->_inuse) return;
    if (!p->owner->misc) return;

    PlayerData *pd = (PlayerData*) p->owner->misc;

    if (p->owner == self){
        return;
    }

    CollisionMask a_mask = attacker->collide.c_mask;
    CollisionMask s_mask = self->collide.c_mask;

    float scale = 1.0f - (self->stats[ARMOR]/200.0f);
    if (scale < 0) scale = 0;
        if (a_mask != s_mask || s_mask == CM_FFA ){
        slog("do damge");
        self->stats[HEALTH] -= p->damage * scale;
        self->stats[ARMOR] = 0;
            if(self->stats[HEALTH] <= 0){
            pd->points++;
            }
        }

}

void apply_passive(Ent *self, Uint8 id) {
    PlayerData *pd = (PlayerData *) self->misc;
    switch(id){

    case 1:
    // gain armor passively when health is near full

        if (self->stats[HEALTH]>=750 && self->stats[ARMOR] < 100){
            self->stats[ARMOR]+=20;
        }

    break;

    case 2:
    // movement speed up

    self->stats[SPEED] = SPEED_CONSTANT*1.5;
    break;
    case 3:
    // reload speed up

    pd->hand->rdebounce -= 1;
    break;
    case 4:
    // jump power
     self->stats[JUMPPOWER] = JUMP_CONSTANT*1.5;
    break;
    case 5:
    // passive regen to a cap
    if (self->stats[HEALTH]<=75){
    self->stats[HEALTH]+=2;
    }
    break;
}
}

void activate_ability(Ent *self, Uint8 id){

    PlayerData *pd = (PlayerData *) self->misc;

    switch(id){

        case 1:
            // gain some armor
            if (self->stats[ARMOR] <= 75){
            self->stats[ARMOR] += 50;
            }
            break;
        case 2:
            // fill ammo
            pd->hand->ammo = pd->hand->apr;
            break;
        case 3:
            // superjump
            self->transform.velocity.x += 1000.0f;
            break;
        case 4:
            // superdash
            self->transform.velocity.y -= 1000.0f;
            break;
        case 5:
            // heal chunk of health
            if (self->stats[HEALTH] <= 75){
            self->stats[HEALTH]+=25;
            }
            break;
    }

}

char* display_points(Ent *self){
    char *buff = malloc(50);

    if (!buff) return NULL;

    snprintf(buff, 50, "Points: %d", get_points(self));

    return buff;
}

Ent *player_new(){
    Ent *self;
    self = ent_new();
    if (!self){
        slog("failed to spawn player ent");
        return NULL;
    }

    int width, height;
    width = 50;
    height = 100;
    self->sprite = gf2d_sprite_load_all(
        "images/body.png",
        width,
        height,
        1,
        0
    );
    //

    // Default Stats
    PlayerData *pd = (PlayerData *) malloc(sizeof(PlayerData));
    // set null weapon
    // empty perks
    // unreachable charge timer
    pd->passiveId = 0;
    pd->activeId = 0;
    pd->activeCharge = 0;
    pd->hillTimer=0;
    pd->passiveCd = -1;
    pd->guns = (Gun*) malloc(sizeof(Gun)*3);
    pd->hand = pd->guns;

    self->misc = pd;
    pd->head = gf2d_sprite_load_all(
        "images/head.png",
        50,
        50,
        1,
        0
    );

    self->stats = (int*) malloc(sizeof(int) * 5);
    self->stats[HEALTH] = 1000;
    self->stats[ARMOR] = 0;
    self->stats[SPEED] = SPEED_CONSTANT;
    self->stats[JUMPPOWER] = JUMP_CONSTANT;
    self->stats[RESPAWNTIMER] = 5.0f;
    self->_tags = TAG_PLAYER;


    self->frame=0;
    self->color = gfc_color8(255,255,255, 255);
    self->transform.velocity = gfc_vector2d(0,0);
    self->transform.position = gfc_vector2d(0,0);
    self->transform.center = gfc_vector2d(0,0);
    gfc_vector2d_add(pd->hTransform.position, self->transform.position, gfc_vector2d(0, 40)); // head transform
    pd->hTransform.scale = gfc_vector2d(1,1);
    pd->hTransform.center = gfc_vector2d(25,25);
    pd->hTransform.rotation = 0;
    self->color = gfc_color8(255,255,255, 255);



    self->collide.c_dim = gfc_vector2d(width/10,height/10);
    self->collide.c_color = GFC_COLOR_RED;
    self->collide.c_mask = NONE;

    pd->hCollide.c_dim = gfc_vector2d(width/10, width/10);

    self->think = player_think;
    self->update = player_update;
    self->draw = player_draw;
    self->free = player_free;
    self->hit = player_hit;

    insert_collision_layer(self);

    load_player_data(self);

    return self;

}

