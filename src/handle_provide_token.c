#include "thales_plugin.h"

// EDIT THIS: Adapt this function to your needs! Remember, the information for tokens are held in
// `msg->token1` and `msg->token2`. If those pointers are `NULL`, this means the ethereum app didn't
// find any info regarding the requested tokens!
void handle_provide_token(void *parameters) {
    ethPluginProvideInfo_t *msg = (ethPluginProvideInfo_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    if (msg->item1) {
        // The Ethereum App found the information for the requested token!
        // Store its decimals.
        context->decimals = msg->item1->token.decimals;
        // Store its ticker.
        strlcpy(context->ticker, (char *) msg->item1->token.ticker, sizeof(context->ticker));

        // Keep track that we found the token.
        context->token_found = true;
    } else {
        context->decimals = WEI_TO_ETHER;

        if (memcmp(context->position, AMM_POSITION_UP, INT256_LENGTH) == 0) {
            strlcpy(context->ticker, "UP", sizeof(context->ticker));
        } else if (memcmp(context->position, AMM_POSITION_DOWN, INT256_LENGTH) == 0) {
            strlcpy(context->ticker, "DOWN", sizeof(context->ticker));
        } else {
            strlcpy(context->ticker, "???", sizeof(context->ticker));
        }

        // The Ethereum App did not manage to find the info for the requested token.
        context->token_found = false;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}