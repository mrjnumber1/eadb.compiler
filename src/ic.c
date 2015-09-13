#include <script.h>

int main(int argc, char * argv[]) {
    script_t * scribe = NULL;

    if (script_init(&scribe,
        "C:\\Users\\trickyloki3\\Desktop\\git\\eadb.compiler\\out\\opt.db",
        "C:\\Users\\trickyloki3\\Desktop\\git\\eadb.compiler\\out\\ea.db",
        "C:\\Users\\trickyloki3\\Desktop\\git\\eadb.compiler\\res\\athena_db.txt",
        EATHENA))
        exit(EXIT_FAILURE);

    while(!item_iterate(scribe->db, &scribe->item)) {
        scribe->offset = 0;

        /* skip empty scripts */
        if (scribe->item.script[0] == '{' && scribe->item.script[1] == '}')
            continue;

        /* compile the item script */
        if (script_lexical(&scribe->token, scribe->item.script) ||
            script_analysis(scribe, &scribe->token, NULL, NULL) ||
            script_translate(scribe)) {
            printf("Error on item %d; %s!\n", scribe->item.id, scribe->item.script);
            break;
        }

        if (scribe->item.id == 1388) {
            script_block_dump(scribe, stderr);
            printf("%s\n", scribe->item.script);
            break;
        }

        if( script_bonus(scribe) ||
            script_generate(scribe, scribe->buffer, &scribe->offset)) {
            script_block_dump(scribe, stderr);
            printf("Error on item %d; %s!\n", scribe->item.id, scribe->item.script);
            break;
        }

        /*printf("%s", scribe->buffer);*/
        script_block_reset(scribe);
    }

    script_deit(&scribe);
    return 0;
}