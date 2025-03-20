// credit to https://github.com/JulienML/BetterChat/ thx fam
const std::map<std::string, std::string> quickchat_ids_to_text = {
    { "Group1Message1", "I got it!" },       // Je l'ai !
    { "Group1Message2", "Need boost!" },     // Besoin de turbo !
    { "Group1Message3", "Take the shot!" },  // Prends-le !
    { "Group1Message4", "Defending." },      // Je d�fends.
    { "Group1Message5", "Go for it!" },      // Vas-y !
    { "Group1Message6", "Centering!" },      // Centre !
    { "Group1Message7", "All yours." },      // Il est pour toi.
    { "Group1Message8", "In position." },    // En position.
    { "Group1Message9", "Incoming!" },       // En approche !
    { "Group1Message10", "Faking." },        // La feinte.
    { "Group1Message11", "Bumping!" },       // Impact !
    { "Group1Message12", "On your left!" },  // Sur ta gauche !
    { "Group1Message13", "On your right!" }, // Sur ta droite !
    { "Group1Message14", "Passing!" },       // La passe !
    { "Group1Message15", "Rotating Up!" },   // Je monte !
    { "Group1Message16", "Rotating back!" }, // Je recule !
    { "Group1Message17", "You have time!" }, // Tu as le temps !

    { "Group2Message1", "Nice shot!" },    // Beau tir !
    { "Group2Message2", "Great pass!" },   // Belle passe !
    { "Group2Message3", "Thanks!" },       // Merci !
    { "Group2Message4", "What a save!" },  // Quel arr�t !
    { "Group2Message5", "Nice one!" },     // Bien vu !
    { "Group2Message6", "What a play!" },  // Quelle intensit� !
    { "Group2Message7", "Great clear!" },  // Beau d�gagement !
    { "Group2Message8", "Nice block!" },   // Super blocage !
    { "Group2Message9", "Nice bump!" },    // Bel impact !
    { "Group2Message10", "Nice demo!" },   // Jolie d�mo !
    { "Group2Message11", "We got this." }, // On assure !

    { "Group3Message1", "OMG!" },         // Oh mon dieu !
    { "Group3Message2", "Noooo!" },       // Noooon !
    { "Group3Message3", "Wow!" },         // Wow !
    { "Group3Message4", "Close one..." }, // C'�tait pas loin...
    { "Group3Message5", "No way!" },      // Pas possible !
    { "Group3Message6", "Holy cow!" },    // S�rieux ?!
    { "Group3Message7", "Whew." },        // Waouh.
    { "Group3Message8", "Siiiick!" },     // Truc de ouf !
    { "Group3Message9", "Calculated." },  // C'est pr�vu.
    { "Group3Message10", "Savage!" },     // Sauvage !
    { "Group3Message11", "Okay." },       // Ok.
    { "Group3Message12", "Yes!" },        // Oui !

    { "Group4Message1", "$#@%!" },       // $#@%!
    { "Group4Message2", "No problem." }, // Pas de probl�mes.
    { "Group4Message3", "Whoops..." },   // Oups...
    { "Group4Message4", "Sorry!" },      // D�sol� !
    { "Group4Message5", "My bad..." },   // Pardon...
    { "Group4Message6", "Oops!" },       // Oups !
    { "Group4Message7", "My fault." },   // Ma faute.

    { "Group5Message1", "gg" },               // gg
    { "Group5Message2", "Well played." },     // Bien jou�.
    { "Group5Message3", "That was fun!" },    // C'�tait cool !
    { "Group5Message4", "Rematch!" },         // On remet �a !
    { "Group5Message5", "One. More. Game." }, // Encore. Une. Partie.
    { "Group5Message6", "What a game!" },     // Quelle partie !
    { "Group5Message7", "Nice moves!" },      // Super d�placements !
    { "Group5Message8", "Everybody dance!" }, // Que tout le monde dance !
    { "Group5Message9", "Party Up?" },        // On groupe ?

    { "Group6Message4", "This is Rocket League!" } // �a c'est Rocket League !
};

struct NetId {
    uint64_t Uid;
    FSceNpId NpId;
    std::string EpicAccountId;
    uint8_t Platform;
    uint8_t SplitscreenID;

    NetId() {}
    NetId(const FUniqueNetId& unreal_id)
      : Uid(unreal_id.Uid)
      , NpId(unreal_id.NpId)
      , EpicAccountId(unreal_id.EpicAccountId.ToString())
      , Platform(unreal_id.Platform)
      , SplitscreenID(unreal_id.SplitscreenID) {}

    FUniqueNetId to_unreal_id() const {
        FUniqueNetId id;
        id.Uid = Uid;
        id.NpId = NpId;
        id.EpicAccountId = Instances.NewFString(EpicAccountId);
        id.Platform = Platform;
        id.SplitscreenID = SplitscreenID;

        return id;
    }
};

struct UidWrapper {
    FUniqueNetId id;

    static inline std::string unreal_id_to_uid_str(const FUniqueNetId& id) {
        return UniqueIDWrapper::FromEpicAccountID(
            id.EpicAccountId.ToString(), id.Uid, static_cast<OnlinePlatform>(id.Platform), id.SplitscreenID)
            .GetIdString();
    }
};

enum class ERankPlaylists : uint8_t { Ones = 0, Twos = 1, Threes = 2, Casual = 3 };
