# Things to conside
1. Right now, the game side (game.h, game.cpp), "knows" about some of the renderer stuff. It actually shouldn't. We should decouple these systems completely so incase we need to
swap the current renderer, we can do it easily
2. To achieve the above outcome we could move the structs: (scene, entity, camera) out of the renderer.