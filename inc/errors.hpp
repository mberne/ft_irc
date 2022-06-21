#ifndef ERRORS_HPP
# define ERRORS_HPP

#include "ircserv.hpp"

# define ERR_UNKNOWNERROR(user, subcommand, info) PROMPT(" 400 ", user) + " " + (subcommand.empty() ? "" : " ") + subcommand) + std::string(" :") + info
// Indicates that the given command/subcommand could not be processed. <subcommand> may repeat for more specific subcommands.

# define ERR_NOSUCHNICK(user, name) PROMPT(" 401 ", user) + " " + name + std::string(" :No such nick/channel")
// Utilisé pour indiquer que le pseudonyme passé en paramètre à la commande n'est pas actuellement utilisé.

# define ERR_NOSUCHSERVER(user, name) PROMPT(" 402 ", user) + " " + name + std::string(" :No such server")
// Utilisé pour indiquer que le nom du serveur donné n'existe pas actuellement.

# define ERR_NOSUCHCHANNEL(user, name) PROMPT(" 403 ", user) + " " + name + std::string(" :No such channel")
// Utilisé pour indiquer que le nom de canal donné est invalide.

# define ERR_CANNOTSENDTOCHAN(user, name) PROMPT(" 404 ", user) + " " + name + std::string(" :Cannot send to channel")
// Envoyé à un utilisateur qui (a) soit n'est pas dans un canal en mode +n ou (b) n'est pas opérateur (ou mode +v) sur un canal en mode +m ; et essaie d'envoyer un PRIVMSG à ce canal.

# define ERR_TOOMANYCHANNELS(user, name) PROMPT(" 405 ", user) + " " + name + std::string(" :You have joined too many channels")
// Envoyé à un utilisateur quand il a atteint le nombre maximal de canaux qu'il est autorisé à accéder simultanément, s'il essaie d'en rejoindre un autre.

# define ERR_WASNOSUCHNICK(user, name) PROMPT(" 406 ", user) + " " + name + std::string(" :There was no such nickname")
// Renvoyé par WHOWAS pour indiquer qu'il n'y a pas d'information dans l'historique concernant ce pseudonyme.

# define ERR_TOOMANYTARGETS(user, name) PROMPT(" 407 ", user) + " " + name + std::string(" :Duplicate recipients. No message delivered")
// Renvoyé à un client qui essaie d'envoyer un PRIVMSG/NOTICE utilisant le format de destination utilisateur@hôte pour lequel utilisateur@hôte a plusieurs occurrences.

# define ERR_NOORIGIN(user) PROMPT(" 409 ", user) + " " + std::string(" :No origin specified")
// Message PING ou PONG sans le paramètre origine qui est obligatoire puisque ces commandes doivent marcher sans préfixe.

# define ERR_NORECIPIENT(user, command) PROMPT(" 411 ", user) + " " + std::string(" :No recipient given (<") + command + std::string(">)")
// Pas de destinataire.

# define ERR_NOTEXTTOSEND(user) PROMPT(" 412 ", user) + " " + std::string(" :No text to send")
// Pas de texte à envoyer.

# define ERR_NOTOPLEVEL(user, domain) PROMPT(" 413 ", user) + " " +  + std::string(" <masque> :No toplevel domain specified" //
// Domaine principal non spécifié.

# define ERR_WILDTOPLEVEL(user, domain) PROMPT(" 414 ", user) + " " +  + std::string(" <masque> :Wildcard in toplevel domain" //
// Joker dans le domaine principal

# define ERR_UNKNOWNCOMMAND(user, command) PROMPT(" 421 ", user) + " " + command + std::string(" :Unknown command")
// Renvoyé à un client enregistré pour indiquer que la commande envoyée est inconnue du serveur.

# define ERR_NOADMININFO(user, name) PROMPT(" 423 ", user) + " " + name + std::string(" :No administrative info available")
// Renvoyé par un serveur en réponse à un message ADMIN quand il y a une erreur lors de la recherche des informations appropriées.

# define ERR_NONICKNAMEGIVEN(user) PROMPT(" 431 ", user) + " " + std::string(" :No nickname given")
// Renvoyé quand un paramètre pseudonyme attendu pour une commande n'est pas fourni.

# define ERR_ERRONEUSNICKNAME(user, name) PROMPT(" 432 ", user) + " " + name + std::string(" :Erroneus nickname")
// Renvoyé après la réception d'un message NICK qui contient des caractères qui ne font pas partie du jeu autorisé. Voir les sections 1 et 2.2 pour les détails des pseudonymes valides.

# define ERR_NICKNAMEINUSE(user, name) PROMPT(" 433 ", user) + " " + name + std::string(" :Nickname is already in use")
// Renvoyé quand le traitement d'un message NICK résulte en une tentative de changer de pseudonyme en un déjà existant.

# define ERR_NICKCOLLISION(user, name) PROMPT(" 436 ", user) + " " + name + std::string(" :Nickname collision KILL")
// Renvoyé par un serveur à un client lorsqu'il détecte une collision de pseudonymes (enregistrement d'un pseudonyme qui existe déjà sur un autre serveur).

# define ERR_USERNOTINCHANNEL(user, nickname, channel) PROMPT(" 441 ", user) + " " + nickname + " " + channel + std::string(" :They aren't on that channel")
// Renvoyé par un serveur pour indiquer que l'utilisateur donné n'est pas dans le canal spécifié.

# define ERR_NOTONCHANNEL(user, name) PROMPT(" 442 ", user) + " " + name + std::string(" :You're not on that channel")
// Renvoyé par le serveur quand un client essaie une commande affectant un canal dont il ne fait pas partie.

# define ERR_NOTREGISTERED(user) PROMPT(" 451 ", user) + std::string(" :You have not registered")
// Retourné par le serveur pour indiquer à un client qu'il doit être enregistré avant que ses commandes soient traitées.

# define ERR_NEEDMOREPARAMS(user, command) PROMPT(" 461 ", user) + " " + command + std::string(" :Not enough parameters")
// Renvoyé par un serveur par de nombreuses commandes, afin d'indiquer que le client n'a pas fourni assez de paramètres.

# define ERR_ALREADYREGISTRED(user) PROMPT(" 462 ", user) + " " + std::string(" :You may not reregister")
// Retourné par le serveur à tout lien qui tente de changer les détails enregistrés (tels que mot de passe et détails utilisateur du second message USER)

# define ERR_NOPERMFORHOST(user) PROMPT(" 463 ", user) + " " + std::string(" :Your host isn't among the privileged")
// Renvoyé à un client qui essaie de s'enregistrer sur un serveur qui n'accepte pas les connexions depuis cet hôte.

# define ERR_PASSWDMISMATCH(user) PROMPT(" 464 ", user) + " " + std::string(" :Password incorrect")
// Retourné pour indiquer l'échec d'une tentative d'enregistrement d'une connexion dû à un mot de passe incorrect ou manquant.

# define ERR_YOUREBANNEDCREEP(user) PROMPT(" 465 ", user) + " " + std::string(" :You are banned from this server")
// Retourné après une tentative de connexion et d'enregistrement sur un serveur configuré explicitement pour vous refuser les connexions.

# define ERR_KEYSET(user, name) PROMPT(" 467 ", user) + " " + name + std::string(" :Channel key already set")
// Clé de canal déjà définie.

# define ERR_CHANNELISFULL(user, name) PROMPT(" 471 ", user) + " " + name + std::string(" :Cannot join channel (+l)")
// Impossible de joindre le canal (+l)

# define ERR_UNKNOWNMODE(user, arg) PROMPT(" 472 ", user) + " " + arg + std::string(" :is unknown mode char to me")
// Mode inconnu.

# define ERR_INVITEONLYCHAN(user, name) PROMPT(" 473 ", user) + " " + name + std::string(" :Cannot join channel (+i)")
// Impossible de joindre le canal (+i).

# define ERR_BANNEDFROMCHAN(user, name) PROMPT(" 474 ", user) + " " + name + std::string(" :Cannot join channel (+b)")
// Impossible de joindre le canal (+b).

# define ERR_BADCHANNELKEY(user, name) PROMPT(" 475 ", user) + " " + name + std::string(" :Cannot join channel (+k)")
// Impossible de joindre le canal (+k).

# define ERR_NOPRIVILEGES(user) PROMPT(" 481 ", user) + " " + std::string(" :Permission Denied- You're not an IRC operator")
// Toute commande qui requiert le privilège d'opérateur pour opérer doit retourner cette erreur pour indiquer son échec.

# define ERR_CHANOPRIVSNEEDED(user, name) PROMPT(" 482 ", user) + " " + name + std::string(" :You're not channel operator")
// Toute commande qui requiert les privilèges 'chanop' (tels les messages MODE) doit retourner ce message à un client qui l'utilise sans être chanop sur le canal spécifié.

# define ERR_CANTKILLSERVER(user) PROMPT(" 483 ", user) + " " + std::string(" :You cant kill a server!")
// Toute tentative d'utiliser la commande KILL sur un serveur doit être refusée et cette erreur renvoyée directement au client.

# define ERR_NOOPERHOST(user) PROMPT(" 491 ", user) + " " + std::string(" :No O-lines for your host")
// Si un client envoie un message OPER et que le serveur n'a pas été configuré pour autoriser les connexions d'opérateurs de cet hôte, cette erreur doit être retournée.

# define ERR_UMODEUNKNOWNFLAG(user) PROMPT(" 501 ", user) + " " + std::string(" :Unknown MODE flag")
// Renvoyé par un serveur pour indiquer que le message MODE a été envoyé avec un pseudonyme et que le mode spécifié n'a pas été identifié.

# define ERR_USERSDONTMATCH(user) PROMPT(" 502 ", user) + " " + std::string(" :Cant change mode for other users")
// Erreur envoyée à tout utilisateur qui essaie de voir ou de modifier le mode utilisateur d'un autre client.

#endif
