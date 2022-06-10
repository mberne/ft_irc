#ifndef RPL_HPP
# define RPL_HPP

#include "ircserv.hpp"

// Les réponses 311 à 313 et 317 à 319 sont toutes générées en réponse à un message WHOIS.
// S'il y a assez de paramètres, le serveur répondant doit soit formuler une réponse parmi les numéros ci-dessus (si le pseudo recherché a été trouvé) ou renvoyer un message d'erreur.
// Le '*' dans RPL_WHOISUSER est là en tant que littéral et non en tant que joker.
// Pour chaque jeu de réponses, seul RPL_WHOISCHANNELS peut apparaître plusieurs fois (pour les longues listes de noms de canaux).
// Les caractères '@' et '+' à côté du nom de canal indiquent si un client est opérateur de canal, ou si on l'a autorisé à parler dans un canal modéré.
// La réponse RPL_ENDOFWHOIS est utilisée pour marquer la fin de la réponse WHOIS.
# define RPL_WHOISUSER(user, client) SERV_NAME + std::string(" 311 ") + user + std::string(" ") + client->getNickname() + std::string(" ") + client->getUser() + std::string(" ") + client->getHost() + std::string(" * :") + client->getRealName()
# define RPL_WHOISSERVER(user, nickname) SERV_NAME + std::string(" 312 ") + user + std::string(" ") + nickname + std::string(" ") + SERV_NAME + std::string(" ") + SERV_INFO
# define RPL_WHOISOPERATOR(user, nickname) SERV_NAME + std::string(" 313 ") + user + std::string(" ") + nickname + std::string(" :is an IRC operator")
# define RPL_WHOISCHANNELS(user, nickname, client) SERV_NAME + std::string(" 319 ") + user + std::string(" ") + nickname + std::string(" :") + client->showChannelList
# define RPL_ENDOFWHOIS(user, nickname) SERV_NAME + std::string(" 318 ") + user + std::string(" ") + nickname + std::string(" :End of /WHOIS list")

// Lorsqu'il répond à un message WHOWAS, un serveur doit utiliser RPL_WHOWASUSER, RPL_WHOISSERVER ou ERR_WASNOSUCHNICK pour chacun des pseudonymes de la liste fournie.
// A la fin de toutes les réponses, il doit y avoir un RPL_ENDOFWHOWAS (même s'il n'y a eu qu'une réponse, et que c'était une erreur).
# define RPL_WHOWASUSER(user, client) SERV_NAME + std::string(" 314 ") + user + std::string(" ") + client->getNickname() + std::string(" ") + client->getUser() + std::string(" ") + client->getHost() + std::string(" * :") + client->getRealName()
# define RPL_ENDOFWHOWAS(user, nickname) SERV_NAME + std::string(" 369 ") + user + std::string(" ") + nickname + std::string(" :End of WHOWAS")

// Les réponses RPL_LISTSTART, RPL_LIST, RPL_LISTEND marquent le début, les réponses proprement dites, et la fin du traitement d'une commande LIST.
// S'il n'y a aucun canal disponible, seules les réponses de début et de fin sont envoyées.
# define RPL_LISTSTART(user) SERV_NAME + std::string(" 321 ") + user + std::string(" Channel :Users Name")
# define RPL_LIST(user, channel) SERV_NAME + std::string(" 322 ") + user + std::string(" ") + channel->getName() + std::string(" ") + channel->clientCount() + std::string(" :") + channel->getTopic() // boucler dessus
# define RPL_LISTEND(user) SERV_NAME + std::string(" 323 ") + user + std::string(" :End of /LIST")

# define RPL_CHANNELMODEIS(user, channel, mode, modeParams) SERV_NAME + std::string(" 324 ") + user + std::string(" ") + channel->getName() + std::string(" ") + mode + (modeParams.empty() ? "" : " " + modeParams)

// Lors de l'envoi d'un message TOPIC pour déterminer le sujet d'un canal, une de ces deux réponses est envoyée.
// Si le sujet est défini, RPL_TOPIC est renvoyée, sinon c'est RPL_NOTOPIC.
# define RPL_NOTOPIC(user, name) SERV_NAME + std::string(" 331 ") + user + std::string(" ") + name + std::string(" :No topic is set")
# define RPL_TOPIC(user, channel) SERV_NAME + std::string(" 332 ") + user + std::string(" ") + channel->getName() + std::string(" :") + channel->getTopic()

// Réponse du serveur indiquant les détails de sa version.
// <version> est la version actuelle du programme utilisé (comprenant le numéro de mise à jour) et <debuglevel> est utilisé pour indiquer si le serveur fonctionne en mode débugage.
// Le champ <commentaire> peut contenir n'importe quel commentaire au sujet de la version, ou des détails supplémentaires sur la version.
# define RPL_VERSION(user) SERV_NAME + std::string(" 351 ") + user + std::string(" ") + SERV_VERSION + std::string(" ") + SERV_NAME + std::string(" :") + SERV_INFO

// La paire RPL_WHOREPLY et RPL_ENDOFWHO est utilisée en réponse à un message WHO.
// Le RPL_WHOREPLY n'est envoyé que s'il y a une correspondance à la requête WHO.
// S'il y a une liste de paramètres fournie avec le message WHO, un RPL_ENDOFWHO doit être envoyé après le traitement de chaque élément de la liste, <nom> étant l'élément.
# define RPL_WHOREPLY(user) SERV_NAME + std::string(" 352 ") + user + std::string(" ") + client->getLastChannelName() + std::string(" ") + client->getUser() + std::string(" ") + client->getHost() + std::string(" ") + SERV_NAME + std::string(" ") + client->getNickname() + std::string(" H") + (client->isOp() ? "*" : "" ) + (client->getChannel(client->getLastChannelName)->isOperator(client) ? "@" : (client->getChannel(client->getLastChannelName)->hasVoice(client) ? "+" : "")) + std::string(" :0 ") + client->getRealName()
# define RPL_ENDOFWHO(user, name) SERV_NAME + std::string(" 315 ") + user + std::string(" ") + name " :End of /WHO list")

// En réponse à un message NAMES, une paire consistant de RPL_NAMREPLY et RPL_ENDOFNAMES est renvoyée par le serveur au client.
// S'il n'y a pas de canal résultant de la requête, seul RPL_ENDOFNAMES est retourné.
// L'exception à cela est lorsqu'un message NAMES est envoyé sans paramètre et que tous les canaux et contenus visibles sont renvoyés en une suite de message RPL_NAMEREPLY avec un RPL_ENDOFNAMES indiquant la fin.
# define RPL_NAMREPLY(user, channel) SERV_NAME + std::string(" 353 ") + user + std::string(" ") + channel->getName() + std::string(" :") + channel->showClientList()
# define RPL_ENDOFNAMES(user, name) SERV_NAME + std::string(" 366 ") + user + std::string(" ") + name + std::string(" :End of /NAMES list")

// Quand il liste les bannissements actifs pour un canal donné, un serveur doit renvoyer la liste en utilisant les messages RPL_BANLIST et RPL_ENDOFBANLIST.
// Un RPL_BANLIST différent doit être utilisé pour chaque identification de bannissement.
// Après avoir listé les identifications de bannissement (s'il y en a), un RPL_ENDOFBANLIST doit être renvoyé.
# define RPL_BANLIST(user, channel, banid) SERV_NAME + std::string(" 367 ") + user + std::string(" ") + channel->getName() + std::string(" ") + banid
# define RPL_ENDOFBANLIST(user, name) SERV_NAME + std::string(" 368 ") + user + std::string(" ") + name + std::string(" :End of channel ban list")

// Un serveur répondant à un message INFO doit envoyer toute sa série d'info en une suite de réponses RPL_INFO, avec un RPL_ENDOFINFO pour indiquer la fin des réponses.
# define RPL_INFO(user, arg) SERV_NAME + std::string(" 371 ") + user + std::string(" ") + arg
# define RPL_ENDOFINFO(user) SERV_NAME + std::string(" 374 ") + user + std::string(" :End of /INFO list")

// RPL_YOUREOPER est renvoyé à un client qui vient d'émettre un message OPER et a obtenu le statut d'opérateur.
# define RPL_YOUREOPER(user) SERV_NAME + std::string(" 381 ") + user + std::string(" :You are now an IRC operator")

// Lorsqu'il répond à un message TIME, un serveur doit répondre en utilisant le format RPL_TIME.
// La chaîne montrant l'heure ne doit contenir que le jour et l'heure corrects.
// Il n'y a pas d'obligation supplémentaire.
# define RPL_TIME(user, currentTime) SERV_NAME + std::string(" 391 ") + user + std::string(" ") + SERV_NAME + std::string(":") + currentTime

// Pour répondre à une requête au sujet du mode du client, RPL_UMODEIS est renvoyé.
# define RPL_STATSUPTIME(user, server) SERV_NAME + std::string(" 242 ") + user + std::string(" :Server Up ") + server->getStartTime()
# define RPL_UMODEIS(user, client) SERV_NAME + std::string(" 221 ") + user + std::string(" ") + client->getMods()
# define RPL_ENDOFSTATS(user, arg) SERV_NAME + std::string(" 219 ") + user + std::string(" ") + arg + std::string(" :End of /STATS report")

// Lorsqu'il répond à un message ADMIN, un serveur doit renvoyer les réponses RLP_ADMINME à RPL_ADMINEMAIL et fournir un texte de message avec chacune.
// Pour RPL_ADMINLOC1, on attend une description de la ville et de l'état où se trouve le serveur, suivie des détails de l'université et du département (RPL_ADMINLOC2), et finalement le contact administratif pour ce serveur (avec obligatoirement une adresse email) dans RPL_ADMINEMAIL.
# define RPL_ADMINME(user) SERV_NAME + std::string(" 256 ") + user + std::string(" ") + SERV_NAME " :Administrative info"
# define RPL_ADMINLOC1(user) SERV_NAME + std::string(" 257 ") + user + std::string(" :") + SERV_LOC1
# define RPL_ADMINLOC2(user) SERV_NAME + std::string(" 258 ") + user + std::string(" :") + SERV_LOC2
# define RPL_ADMINEMAIL(user) SERV_NAME + std::string(" 259 ") + user + std::string(" :") + SERV_ADMIN + std::string(" ") + SERV_ADMIN_EMAIL

#endif
