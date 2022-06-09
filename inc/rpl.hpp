#ifndef RPL_HPP
# define RPL_HPP

#include "ircserv.hpp"

// Les réponses 311 à 313 et 317 à 319 sont toutes générées en réponse à un message WHOIS.
// S'il y a assez de paramètres, le serveur répondant doit soit formuler une réponse parmi les numéros ci-dessus (si le pseudo recherché a été trouvé) ou renvoyer un message d'erreur.
// Le '*' dans RPL_WHOISUSER est là en tant que littéral et non en tant que joker.
// Pour chaque jeu de réponses, seul RPL_WHOISCHANNELS peut apparaître plusieurs fois (pour les longues listes de noms de canaux).
// Les caractères '@' et '+' à côté du nom de canal indiquent si un client est opérateur de canal, ou si on l'a autorisé à parler dans un canal modéré.
// La réponse RPL_ENDOFWHOIS est utilisée pour marquer la fin de la réponse WHOIS.
# define RPL_WHOISUSER(user, client) SERV_NAME + " 311 " + user + " " + client->getNickname() + " " + client->getUser() + " " + client->getHost() + " * :" + client->getRealName()
# define RPL_WHOISSERVER(user, nickname) SERV_NAME + " 312 " + user + " " + nickname + " " + SERV_NAME + " " + SERV_INFO
# define RPL_WHOISOPERATOR(user, nickname) SERV_NAME + " 313 " + user + " " + nickname + " :is an IRC operator"
# define RPL_WHOISCHANNELS(user, nickname) SERV_NAME + " 319 " + user + " " + nickname + " :{[@|+]<canal><espace>}" // pthomas
# define RPL_ENDOFWHOIS(user, nickname) SERV_NAME + " 318 " + user + " " + nickname + " :End of /WHOIS list"

// Lorsqu'il répond à un message WHOWAS, un serveur doit utiliser RPL_WHOWASUSER, RPL_WHOISSERVER ou ERR_WASNOSUCHNICK pour chacun des pseudonymes de la liste fournie.
// A la fin de toutes les réponses, il doit y avoir un RPL_ENDOFWHOWAS (même s'il n'y a eu qu'une réponse, et que c'était une erreur).
# define RPL_WHOWASUSER(user, client) SERV_NAME + " 314 " + user + " " + client->getNickname() + " " + client->getUser() + " " + client->getHost() + " * :" + client->getRealName()
# define RPL_ENDOFWHOWAS(user, nickname) SERV_NAME + " 369 " + user + " " + nickname + " :End of WHOWAS"

// Les réponses RPL_LISTSTART, RPL_LIST, RPL_LISTEND marquent le début, les réponses proprement dites, et la fin du traitement d'une commande LIST.
// S'il n'y a aucun canal disponible, seules les réponses de début et de fin sont envoyées.
# define RPL_LISTSTART(user) SERV_NAME + " 321 " + user + " Channel :Users Name"
# define RPL_LIST(user, channel) SERV_NAME + " 322 " + user + " " + channel->getName() + " " + channel->clientCount() + " :" + channel->getTopic() // boucler dessus
# define RPL_LISTEND(user) SERV_NAME + " 323 " + user + " :End of /LIST"

# define RPL_CHANNELMODEIS(user, channel) SERV_NAME + " 324 " + user + " " + channel->getName() + " <mode> <paramètres de mode>" // pthomas

// Lors de l'envoi d'un message TOPIC pour déterminer le sujet d'un canal, une de ces deux réponses est envoyée.
// Si le sujet est défini, RPL_TOPIC est renvoyée, sinon c'est RPL_NOTOPIC.
# define RPL_NOTOPIC(user, name) SERV_NAME + " 331 " + user + " " + name " :No topic is set"
# define RPL_TOPIC(user, channel) SERV_NAME + " 332 " + user + " " + channel->getName() + " :" + channel->getTopic()

// Réponse du serveur indiquant les détails de sa version.
// <version> est la version actuelle du programme utilisé (comprenant le numéro de mise à jour) et <debuglevel> est utilisé pour indiquer si le serveur fonctionne en mode débugage.
// Le champ <commentaire> peut contenir n'importe quel commentaire au sujet de la version, ou des détails supplémentaires sur la version.
# define RPL_VERSION(user) SERV_NAME + " 351 " + user + " " + SERV_VERSION + " " + SERV_NAME + " :" + SERV_INFO

// La paire RPL_WHOREPLY et RPL_ENDOFWHO est utilisée en réponse à un message WHO.
// Le RPL_WHOREPLY n'est envoyé que s'il y a une correspondance à la requête WHO.
// S'il y a une liste de paramètres fournie avec le message WHO, un RPL_ENDOFWHO doit être envoyé après le traitement de chaque élément de la liste, <nom> étant l'élément.
# define RPL_WHOREPLY(user) SERV_NAME + " 352 " + user + " " + client->getLastChannelName() + " " + client->getUser() + " " + client->getHost() + " " + SERV_NAME + " " + client->getNickname() + " <H|G>[*][@|+]" + " :0 " + client->getRealName() // pthomas
# define RPL_ENDOFWHO(user, name) SERV_NAME + " 315 " + user + " " + name " :End of /WHO list"

// En réponse à un message NAMES, une paire consistant de RPL_NAMREPLY et RPL_ENDOFNAMES est renvoyée par le serveur au client.
// S'il n'y a pas de canal résultant de la requête, seul RPL_ENDOFNAMES est retourné.
// L'exception à cela est lorsqu'un message NAMES est envoyé sans paramètre et que tous les canaux et contenus visibles sont renvoyés en une suite de message RPL_NAMEREPLY avec un RPL_ENDOFNAMES indiquant la fin.
# define RPL_NAMREPLY(user, channel) SERV_NAME + " 353 " + user + " " + channel->getName() + " :[[@|+]<pseudo> [[@|+]<pseudo> [...]]]" // pthomas
# define RPL_ENDOFNAMES(user, name) SERV_NAME + " 366 " + user + " " + name + " :End of /NAMES list"

// Quand il liste les bannissements actifs pour un canal donné, un serveur doit renvoyer la liste en utilisant les messages RPL_BANLIST et RPL_ENDOFBANLIST.
// Un RPL_BANLIST différent doit être utilisé pour chaque identification de bannissement.
// Après avoir listé les identifications de bannissement (s'il y en a), un RPL_ENDOFBANLIST doit être renvoyé.
# define RPL_BANLIST(user, channel) SERV_NAME + " 367 " + user + " " + channel->getName() + " " + <identification de bannissement> // pthomas
# define RPL_ENDOFBANLIST(user, name) SERV_NAME + " 368 " + user + " " + name + " :End of channel ban list"

// Un serveur répondant à un message INFO doit envoyer toute sa série d'info en une suite de réponses RPL_INFO, avec un RPL_ENDOFINFO pour indiquer la fin des réponses.
# define RPL_INFO(user, arg) SERV_NAME + " 371 " + user + " " + arg // boucle sur les macro SERV a mettre en arg
# define RPL_ENDOFINFO(user) SERV_NAME + " 374 " + user + " :End of /INFO list"

// RPL_YOUREOPER est renvoyé à un client qui vient d'émettre un message OPER et a obtenu le statut d'opérateur.
# define RPL_YOUREOPER(user) SERV_NAME + " 381 " + user + " :You are now an IRC operator"

// Lorsqu'il répond à un message TIME, un serveur doit répondre en utilisant le format RPL_TIME.
// La chaîne montrant l'heure ne doit contenir que le jour et l'heure corrects.
// Il n'y a pas d'obligation supplémentaire.
# define RPL_TIME(user, currentTime) SERV_NAME + " 391 " + user + " " + SERV_NAME + ":" + currentTime

// Pour répondre à une requête au sujet du mode du client, RPL_UMODEIS est renvoyé.
# define RPL_STATSUPTIME(user, server) SERV_NAME + " 242 " + user + " :Server Up " + server->getStartTime()
# define RPL_UMODEIS(user, client) SERV_NAME + " 221 " + user + " <chaîne de mode utilisateur>" // pthomas
# define RPL_ENDOFSTATS(user, arg) SERV_NAME + " 219 " + user + " " + arg + " :End of /STATS report"

// Lorsqu'il répond à un message ADMIN, un serveur doit renvoyer les réponses RLP_ADMINME à RPL_ADMINEMAIL et fournir un texte de message avec chacune.
// Pour RPL_ADMINLOC1, on attend une description de la ville et de l'état où se trouve le serveur, suivie des détails de l'université et du département (RPL_ADMINLOC2), et finalement le contact administratif pour ce serveur (avec obligatoirement une adresse email) dans RPL_ADMINEMAIL.
# define RPL_ADMINME(user) SERV_NAME + " 256 " + user + " " + SERV_NAME " :Administrative info"
# define RPL_ADMINLOC1(user) SERV_NAME + " 257 " + user + " :" + SERV_LOC1
# define RPL_ADMINLOC2(user) SERV_NAME + " 258 " + user + " :" + SERV_LOC2
# define RPL_ADMINEMAIL(user) SERV_NAME + " 259 " + user + " :" + SERV_ADMIN + " " + SERV_ADMIN_EMAIL

#endif
