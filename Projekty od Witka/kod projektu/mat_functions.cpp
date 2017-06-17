#include "mat_functions.h"

namespace std{

    static float absSin(float angle, float thresholdToOne) {
         float sinus = abs(sin(angle));
         if (sinus > thresholdToOne) sinus = thresholdToOne;
         sinus = sinus * (1 / thresholdToOne);
        return sinus;
     }

    const float EngineParams::r = 1.0f;
    const float EngineParams::length = 2.4f;
    const float EngineParams::alfa = r / length;
    const float EngineParams::valveAngle = PI / 6;
    const float EngineParams::sinCut = 0.75f;



    mat4 matRotateXYZ(float angle_x, float angle_y, float angle_z) {
        mat4 obr = rotate(mat4(1), angle_x, vec3(1.0f, 0.0f, 0.0f));
        obr = rotate(obr, angle_y, vec3(0.0f, 1.0f, 0.0f));
        obr = rotate(obr, angle_z, vec3(0.0f, 0.0f, 1.0f));
     //obr = rotate(obr, angle_x, vec3(1.0f, 0.0f, 0.0f));

        return obr;
    }

     mat4 mPiston(float pos_x, float pos_z, float max_y, float engine_angle) {
        //if (angle < PI)
        float pos_y = max_y -(EngineParams::r * ((1 - cos(engine_angle)) + (EngineParams::alfa / 4 * (1 - cos(2*engine_angle)))));
            //else pos_y = max_y - delta_y * ((angle - PI) / PI);
        //pos_y = max_y - delta_y * (angle / (PI));
        //cout << pos_y << endl;
        return translate(mat4(1), vec3(pos_x, pos_y, pos_z));
     }

     mat4 mCrank(float pos_x, float pos_z, float max_y, float engine_angle) {
         float top_angle = acos(1 - (EngineParams::alfa * EngineParams::alfa * sin(engine_angle) * sin(engine_angle))/2);
         if(sin(engine_angle) < 0) top_angle = 2*PI - top_angle;
         //cout << sin(engine_angle) << endl;

         //float delta_y = max_y - min_y;
         float pos_y = max_y -(EngineParams::r * ((1 - cos(engine_angle)) + (EngineParams::alfa / 4 * (1 - cos(2*engine_angle)))));
         //float top_angle = max_top_angle * -cos(engine_angle + (PI / 2));
         //cout << top_angle << endl;

         mat4 M =  translate(mat4(1), vec3(pos_x, pos_y, pos_z));
         return rotate(M, top_angle, vec3(1.0f, 0.0f, 0.0f));

     }

     mat4 mValve(float pos_x, float pos_z, float min_y, float delta_y, float engine_angle, ValveDirection direction) {
        float pos_y;
        float sinus = sin(engine_angle);
        float sinus_div = sin(engine_angle / 2.0f);
        mat4 M = mat4(1);


        pos_y = min_y;
        switch (direction) {
            case valve_in: {
                if (sinus_div > 0) {
                    if (sinus < 0) {
                        pos_y = min_y + delta_y * absSin(sinus, EngineParams::sinCut);
                        pos_z = pos_z - delta_y * absSin(sinus, EngineParams::sinCut) * EngineParams::valveAngle;
                    }
                }
            } break;
            case valve_out: {
                if (sinus_div < 0) {
                    if (sinus > 0) {
                        pos_y = min_y + delta_y * absSin(sinus, EngineParams::sinCut);
                        pos_z = pos_z + delta_y * absSin(sinus, EngineParams::sinCut)* EngineParams::valveAngle;
                    }
                }
            } break;
        }
        //cout << pos_y << endl;
        M =  translate(M, vec3(pos_x,pos_y, pos_z));

        switch (direction) {
            case valve_in: {
                M = rotate(M, -EngineParams::valveAngle, vec3(1.0f, 0.0f, 0.0f));
            } break;
            case valve_out: {
                M = rotate(M, EngineParams::valveAngle, vec3(1.0f, 0.0f, 0.0f));
            } break;
        }
        return M;
     }

    mat4 mValveParticle(vec3 pos, ValveDirection direction) {
        mat4 M =  translate(mat4(1), pos);
        switch (direction) {
            case valve_in: {
                M = rotate(M, -EngineParams::valveAngle, vec3(1.0f, 0.0f, 0.0f));
            } break;
            case valve_out: {
                M = rotate(M, (-PI)+EngineParams::valveAngle, vec3(1.0f, 0.0f, 0.0f));
            } break;
        }
        return M;
    }

    mat4 mPistonParticle(vec3 pos) {
        return translate(mat4(1), pos);
    }
    Direction ValveParticleDir(float engine_angle, ValveDirection direction) {
        Direction dir = dir_none;
        float sinus = sin(engine_angle);
        float sinus_div = sin(engine_angle / 2.0f);

        switch (direction) {
            case valve_in: {
                if (sinus_div > 0) {
                    if (sinus < 0) {
                        dir = dir_down;
                    }
                }
            } break;
            case valve_out: {
                if (sinus_div < 0) {
                    if (sinus > 0) {
                        dir = dir_down;
                    }
                }
            } break;
        }
        return dir;
    }
    float ValveParticlePercent(float engine_angle, float minPercent, ValveDirection direction) {
        float sinus = sin(engine_angle);
        float sinus_div = sin(engine_angle / 2.0f);

        float delta_percent = 1.0f - minPercent;
        float percent = minPercent;

        switch (direction) {
            case valve_in: {
                if (sinus_div > 0) {
                    if (sinus < 0) {
                        percent = minPercent + delta_percent * absSin(sinus, EngineParams::sinCut);
                    }
                }
            } break;
            case valve_out: {
                if (sinus_div < 0) {
                    if (sinus > 0) {
                        percent = minPercent + delta_percent * absSin(sinus, EngineParams::sinCut);
                    }
                }
            } break;
        }
        return percent;
    }

    float PistonParticlePercent(float engine_angle, float minPercent) {
        float toFullPercent = 1.0f - minPercent;
        return minPercent + (toFullPercent / 2.0f * ((1 - cos(engine_angle)) + (EngineParams::alfa / 4 * (1 - cos(2*engine_angle)))));
    }
    vec3 PistonParticleColor(float engine_angle) {
        float sinus = sin(engine_angle);
        float sin_div = sin(engine_angle / 2.0f);
        vec3 color;
        //pierwszy obrot
        if (sin_div > 0) {
            // pierwsze pol obrotu
            if (sinus > 0) {
                color = vec3(0.0, 1.0, 1.0);
            } else {
                color = vec3(0.0, sin_div, 1.0);
            }
        } else {
        //drugi obrot
            if (sinus > 0) {
                color = vec3(1.0, 1.0 + (sin_div), 0.0);
            } else {
                color = vec3(0.5 - (sin_div / 2), 0.5 + (sin_div / 2), 0.5 + (sin_div / 2));
            }
        }
        return color;
    }
    /* float normalizeAngle(float angle, float max_angle) {
         while (angle > max_angle) angle -= max_angle;
         while (angle < -max_angle) angle += max_angle;
         return angle;
     }*/

}
