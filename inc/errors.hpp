#ifndef ERRORS_HPP
# define ERRORS_HPP

#include "ircserv.hpp"

# define ERR_NOSUCHNICK(user, command, name) SERV_NAME + " 401 " + user + " " + command + " " + name + " :No such nick/channel"
// Utilisé pour indiquer que le pseudonyme passé en paramètre à la commande n'est pas actuellement utilisé.

# define ERR_NOSUCHSERVER(user, command, name) SERV_NAME + " 402 " + user + " " + command + " " + name + " :No such server"
// Utilisé pour indiquer que le nom du serveur donné n'existe pas actuellement.

# define ERR_NOSUCHCHANNEL(user, command, name) SERV_NAME + " 403 " + user + " " + command + " " + name + " :No such channel"
// Utilisé pour indiquer que le nom de canal donné est invalide.

# define ERR_CANNOTSENDTOCHAN(user, command, name) SERV_NAME + " 404 " + user + " " + command + " " + name + " :Cannot send to channel"
// Envoyé à un utilisateur qui (a) soit n'est pas dans un canal en mode +n ou (b) n'est pas opérateur (ou mode +v) sur un canal en mode +m ; et essaie d'envoyer un PRIVMSG à ce canal.

# define ERR_TOOMANYCHANNELS(user, command, name) SERV_NAME + " 405 " + user + " " + command + " " + name + " :You have joined too many channels"
// Envoyé à un utilisateur quand il a atteint le nombre maximal de canaux qu'il est autorisé à accéder simultanément, s'il essaie d'en rejoindre un autre.

# define ERR_WASNOSUCHNICK(user, command, name) SERV_NAME + " 406 " + user + " " + command + " " + name + " :There was no such nickname"
// Renvoyé par WHOWAS pour indiquer qu'il n'y a pas d'information dans l'historique concernant ce pseudonyme.

# define ERR_TOOMANYTARGETS(user, command, name) SERV_NAME + " 407 " + user + " " + command + " " + name + " :Duplicate recipients. No message delivered"
// Renvoyé à un client qui essaie d'envoyer un PRIVMSG/NOTICE utilisant le format de destination utilisateur@hôte pour lequel utilisateur@hôte a plusieurs occurrences.

# define ERR_NOORIGIN(user, command) SERV_NAME + " 409 " + user + " " + command + " :No origin specified"
// Message PING ou PONG sans le paramètre origine qui est obligatoire puisque ces commandes doivent marcher sans préfixe.

# define ERR_NORECIPIENT(user, command) SERV_NAME + " 411 " + user + " " + command + " :No recipient given (<" + command + ">)"
// Pas de destinataire.

# define ERR_NOTEXTTOSEND(user, command) SERV_NAME + " 412 " + user + " " + command + " :No text to send"
// Pas de texte à envoyer.

# define ERR_NOTOPLEVEL(user, command, domain) SERV_NAME + " 413 " + user + " " + command + " " +  + " <masque> :No toplevel domain specified" //
// Domaine principal non spécifié.

# define ERR_WILDTOPLEVEL(user, command, domain) SERV_NAME + " 414 " + user + " " + command + " " +  + " <masque> :Wildcard in toplevel domain" //
// Joker dans le domaine principal

# define ERR_UNKNOWNCOMMAND(user, command) SERV_NAME + " 421 " + user + " " + command + " :Unknown command"
// Renvoyé à un client enregistré pour indiquer que la commande envoyée est inconnue du serveur.

# define ERR_NOADMININFO(user, command, name) SERV_NAME + " 423 " + user + " " + command + " " + name + " :No administrative info available"
// Renvoyé par un serveur en réponse à un message ADMIN quand il y a une erreur lors de la recherche des informations appropriées.

# define ERR_NONICKNAMEGIVEN(user, command) SERV_NAME + " 431 " + user + " " + command + " :No nickname given"
// Renvoyé quand un paramètre pseudonyme attendu pour une commande n'est pas fourni.

# define ERR_ERRONEUSNICKNAME(user, command, name) SERV_NAME + " 432 " + user + " " + command + " " + name + " :Erroneus nickname"
// Renvoyé après la réception d'un message NICK qui contient des caractères qui ne font pas partie du jeu autorisé. Voir les sections 1 et 2.2 pour les détails des pseudonymes valides.

# define ERR_NICKNAMEINUSE(user, command, name) SERV_NAME + " 433 " + user + " " + command + " " + name + " :Nickname is already in use"
// Renvoyé quand le traitement d'un message NICK résulte en une tentative de changer de pseudonyme en un déjà existant.

# define ERR_NICKCOLLISION(user, command, name) SERV_NAME + " 436 " + user + " " + command + " " + name + " :Nickname collision KILL"
// Renvoyé par un serveur à un client lorsqu'il détecte une collision de pseudonymes (enregistrement d'un pseudonyme qui existe déjà sur un autre serveur).

# define ERR_USERNOTINCHANNEL(user, command, nickname, channel) SERV_NAME + " 441 " + user + " " + command + " " + nickname + " " + channel " :They aren't on that channel"
// Renvoyé par un serveur pour indiquer que l'utilisateur donné n'est pas dans le canal spécifié.

# define ERR_NOTONCHANNEL(user, command, name) SERV_NAME + " 442 " + user + " " + command + " " + name + " :You're not on that channel"
// Renvoyé par le serveur quand un client essaie une commande affectant un canal dont il ne fait pas partie.

# define ERR_NOTREGISTERED(user, command) SERV_NAME + " 451 " + user + " " + command + " :You have not registered"
// Retourné par le serveur pour indiquer à un client qu'il doit être enregistré avant que ses commandes soient traitées.

# define ERR_NEEDMOREPARAMS(user, command) SERV_NAME + " 461 " + user + " " + command + " :Not enough parameters"
// Renvoyé par un serveur par de nombreuses commandes, afin d'indiquer que le client n'a pas fourni assez de paramètres.

# define ERR_ALREADYREGISTRED(user, command) SERV_NAME + " 462 " + user + " " + command + " :You may not reregister"
// Retourné par le serveur à tout lien qui tente de changer les détails enregistrés (tels que mot de passe et détails utilisateur du second message USER)

# define ERR_NOPERMFORHOST(user, command) SERV_NAME + " 463 " + user + " " + command + " :Your host isn't among the privileged"
// Renvoyé à un client qui essaie de s'enregistrer sur un serveur qui n'accepte pas les connexions depuis cet hôte.

# define ERR_PASSWDMISMATCH(user, command) SERV_NAME + " 464 " + user + " " + command + " :Password incorrect"
// Retourné pour indiquer l'échec d'une tentative d'enregistrement d'une connexion dû à un mot de passe incorrect ou manquant.

# define ERR_YOUREBANNEDCREEP(user, command) SERV_NAME + " 465 " + user + " " + command + " :You are banned from this server"
// Retourné après une tentative de connexion et d'enregistrement sur un serveur configuré explicitement pour vous refuser les connexions.

# define ERR_KEYSET(user, command, name) SERV_NAME + " 467 " + user + " " + command + " " + name + " :Channel key already set"
// Clé de canal déjà définie.

# define ERR_CHANNELISFULL(user, command, name) SERV_NAME + " 471 " + user + " " + command + " " + name + " :Cannot join channel (+l)"
// Impossible de joindre le canal (+l)

# define ERR_UNKNOWNMODE(user, command, arg) SERV_NAME + " 472 " + user + " " + command + " " + arg + " :is unknown mode char to me"
// Mode inconnu.

# define ERR_INVITEONLYCHAN(user, command, name) SERV_NAME + " 473 " + user + " " + command + " " + name + " :Cannot join channel (+i)"
// Impossible de joindre le canal (+i).

# define ERR_BANNEDFROMCHAN(user, command, name) SERV_NAME + " 474 " + user + " " + command + " " + name + " :Cannot join channel (+b)"
// Impossible de joindre le canal (+b).

# define ERR_BADCHANNELKEY(user, command, name) SERV_NAME + " 475 " + user + " " + command + " " + name + " :Cannot join channel (+k)"
// Impossible de joindre le canal (+k).

# define ERR_NOPRIVILEGES(user, command) SERV_NAME + " 481 " + user + " " + command + " :Permission Denied- You're not an IRC operator"
// Toute commande qui requiert le privilège d'opérateur pour opérer doit retourner cette erreur pour indiquer son échec.

# define ERR_CHANOPRIVSNEEDED(user, command, name) SERV_NAME + " 482 " + user + " " + command + " " + name + " :You're not channel operator"
// Toute commande qui requiert les privilèges 'chanop' (tels les messages MODE) doit retourner ce message à un client qui l'utilise sans être chanop sur le canal spécifié.

# define ERR_CANTKILLSERVER(user, command) SERV_NAME + " 483 " + user + " " + command + " :You cant kill a server!"
// Toute tentative d'utiliser la commande KILL sur un serveur doit être refusée et cette erreur renvoyée directement au client.

# define ERR_NOOPERHOST(user, command) SERV_NAME + " 491 " + user + " " + command + " :No O-lines for your host"
// Si un client envoie un message OPER et que le serveur n'a pas été configuré pour autoriser les connexions d'opérateurs de cet hôte, cette erreur doit être retournée.

# define ERR_UMODEUNKNOWNFLAG(user, command) SERV_NAME + " 501 " + user + " " + command + " :Unknown MODE flag"
// Renvoyé par un serveur pour indiquer que le message MODE a été envoyé avec un pseudonyme et que le mode spécifié n'a pas été identifié.

# define ERR_USERSDONTMATCH(user, command) SERV_NAME + " 502 " + user + " " + command + " :Cant change mode for other users"
// Erreur envoyée à tout utilisateur qui essaie de voir ou de modifier le mode utilisateur d'un autre client.

#endif
