EventScript_RogueReplaceOrReleaseMon::
    msgbox RogueText_PartyFullChoice, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, NO, EventScript_RogueDeclinedReplace
    special ChoosePartyMon
    waitstate
    special Special_ReplacePartyMon
    end

EventScript_RogueDeclinedReplace::
    end

RogueText_PartyFullChoice:
    .string "Your party's full! Replace a\nteam member with this Pokémon?$"