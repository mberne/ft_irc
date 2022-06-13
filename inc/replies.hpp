#ifndef RPL_HPP
# define RPL_HPP

#include "ircserv.hpp"

# define RPL_WELCOME(user)		PROMPT(" 001 ", user) + std::string(" :Welcome to the ") + SERV_NAME + std::string(" Network, ") + user
# define RPL_YOURHOST(user)		PROMPT(" 002 ", user) + std::string(" :Your host is ") + SERV_NAME + std::string(", running version ") + SERV_VERSION
# define RPL_CREATED(user)		PROMPT(" 003 ", user) + std::string(" :This server was created ") + SERV_CREATION
# define RPL_MYINFO(user)		PROMPT(" 004 ", user) + std::string(" ") + SERV_NAME + std::string(" ") + SERV_VERSION + std::string(" io opsitnmlbvk")
# define RPL_ISUPPORT(user)		PROMPT(" 005 ", user) + std::string(" MAX_PORT=") + MAX_PORT + \
														std::string(" CHANNEL_LIMIT=") + CHANNEL_LIMIT + \
														std::string(" CLIENT_LIMIT=") + CLIENT_LIMIT + \
														std::string(" CHANNEL_LIMIT_PER_CLIENT=") + CHANNEL_LIMIT_PER_CLIENT + \
														std::string(" MAX_NICKNAME_LENGTH=") + MAX_NICKNAME_LENGTH + \
														std::string(" MAX_CHANNEL_LENGTH=") + MAX_CHANNEL_LENGTH + \
														std::string(" MAX_TOPIC_LENGTH=") + MAX_TOPIC_LENGTH + \
														std::string(" MAX_MESSAGE_LENGTH=") + MAX_MESSAGE_LENGTH
																	
// Pour répondre à une requête au sujet du mode du client, RPL_UMODEIS est renvoyé.
# define RPL_ENDOFSTATS(user, arg)		PROMPT(" 219 ", user) + std::string(" ") + arg + std::string(" :End of /STATS report")
# define RPL_UMODEIS(user, client)		PROMPT(" 221 ", user) + std::string(" ") + client->getMods()
# define RPL_STATSUPTIME(user, server)	PROMPT(" 242 ", user) + std::string(" :Server Up ") + server->getStartTime()

// Réponses à LUSERS
# define RPL_LUSERCLIENT(user, server)	PROMPT(" 251 ", user) + std::string(" :There are ") + server->getAllClients().size() + std::string(" users")
# define RPL_LUSEROP(user)				PROMPT(" 252 ", user) + std::string(" <ops> :operator(s) online") // getOpsNumber()
# define RPL_LUSERUNKNOWN(user)			PROMPT(" 253 ", user) + std::string(" <connections> :unknown connection(s)") // getNonRegisteredNumber()
# define RPL_LUSERCHANNELS(user)		PROMPT(" 254 ", user) + std::string(" ") + server->getAllChannels().size() + std::string(":channels formed")

// Lorsqu'il répond à un message ADMIN, un serveur doit renvoyer les réponses RLP_ADMINME à RPL_ADMINEMAIL et fournir un texte de message avec chacune.
// Pour RPL_ADMINLOC1, on attend une description de la ville et de l'état où se trouve le serveur, suivie des détails de l'université et du département (RPL_ADMINLOC2), et finalement le contact administratif pour ce serveur (avec obligatoirement une adresse email) dans RPL_ADMINEMAIL.
# define RPL_ADMINME(user)		PROMPT(" 256 ", user) + std::string(" ") + SERV_NAME " :Administrative info"
# define RPL_ADMINLOC1(user)	PROMPT(" 257 ", user) + std::string(" :") + SERV_LOC1
# define RPL_ADMINLOC2(user)	PROMPT(" 258 ", user) + std::string(" :") + SERV_LOC2
# define RPL_ADMINEMAIL(user)	PROMPT(" 259 ", user) + std::string(" :") + SERV_ADMIN + std::string(" ") + SERV_ADMIN_EMAIL

// Les réponses 311 à 313 et 317 à 319 sont toutes générées en réponse à un message WHOIS.
// S'il y a assez de paramètres, le serveur répondant doit soit formuler une réponse parmi les numéros ci-dessus (si le pseudo recherché a été trouvé) ou renvoyer un message d'erreur.
// Le '*' dans RPL_WHOISUSER est là en tant que littéral et non en tant que joker.
// Pour chaque jeu de réponses, seul RPL_WHOISCHANNELS peut apparaître plusieurs fois (pour les longues listes de noms de canaux).
// Les caractères '@' et '+' à côté du nom de canal indiquent si un client est opérateur de canal, ou si on l'a autorisé à parler dans un canal modéré.
// La réponse RPL_ENDOFWHOIS est utilisée pour marquer la fin de la réponse WHOIS.
# define RPL_WHOISUSER(user, client)				PROMPT(" 311 ", user) + std::string(" ") + client->getNickname() + std::string(" ") + client->getUser() + std::string(" ") + client->getHost() + std::string(" * :") + client->getRealName()
# define RPL_WHOISSERVER(user, nickname)			PROMPT(" 312 ", user) + std::string(" ") + nickname + std::string(" ") + SERV_NAME + std::string(" ") + SERV_INFO
# define RPL_WHOISOPERATOR(user, nickname)			PROMPT(" 313 ", user) + std::string(" ") + nickname + std::string(" :is an IRC operator")
# define RPL_ENDOFWHOIS(user, nickname)				PROMPT(" 318 ", user) + std::string(" ") + nickname + std::string(" :End of /WHOIS list")
# define RPL_WHOISCHANNELS(user, nickname, client)	PROMPT(" 319 ", user) + std::string(" ") + nickname + std::string(" :") + client->showChannelList

// Les réponses RPL_LISTSTART, RPL_LIST, RPL_LISTEND marquent le début, les réponses proprement dites, et la fin du traitement d'une commande LIST.
// S'il n'y a aucun canal disponible, seules les réponses de début et de fin sont envoyées.
# define RPL_LISTSTART(user)		PROMPT(" 321 ", user) + std::string(" Channel :Users Name")
# define RPL_LIST(user, channel)	PROMPT(" 322 ", user) + std::string(" ") + channel->getName() + std::string(" ") + channel->clientCount() + std::string(" :") + channel->getTopic() // boucler dessus
# define RPL_LISTEND(user)			PROMPT(" 323 ", user) + std::string(" :End of /LIST")

# define RPL_CHANNELMODEIS(user, channel, mode, modeParams)	PROMPT(" 324 ", user) + std::string(" ") + channel->getName() + std::string(" ") + mode + (modeParams.empty() ? "" : " " + modeParams)

// Lors de l'envoi d'un message TOPIC pour déterminer le sujet d'un canal, une de ces deux réponses est envoyée.
// Si le sujet est défini, RPL_TOPIC est renvoyée, sinon c'est RPL_NOTOPIC.
# define RPL_NOTOPIC(user, name)	PROMPT(" 331 ", user) + std::string(" ") + name + std::string(" :No topic is set")
# define RPL_TOPIC(user, channel)	PROMPT(" 332 ", user) + std::string(" ") + channel->getName() + std::string(" :") + channel->getTopic()

// La paire RPL_WHOREPLY et RPL_ENDOFWHO est utilisée en réponse à un message WHO.
// Le RPL_WHOREPLY n'est envoyé que s'il y a une correspondance à la requête WHO.
// S'il y a une liste de paramètres fournie avec le message WHO, un RPL_ENDOFWHO doit être envoyé après le traitement de chaque élément de la liste, <nom> étant l'élément.
# define RPL_WHOREPLY(user, client)		PROMPT(" 352 ", user) + std::string(" ") + client->getLastChannelName() + std::string(" ") + client->getUser() + std::string(" ") + client->getHost() + std::string(" ") + SERV_NAME + std::string(" ") + client->getNickname() + std::string(" H") + (client->isOp() ? "*" : "" ) + (client->getChannel(client->getLastChannelName)->isOperator(client) ? "@" : (client->getChannel(client->getLastChannelName)->hasVoice(client) ? "+" : "")) + std::string(" :0 ") + client->getRealName()
# define RPL_ENDOFWHO(user, name)		PROMPT(" 315 ", user) + std::string(" ") + name + " :End of /WHO list"

// Réponse du serveur indiquant les détails de sa version.
// <version> est la version actuelle du programme utilisé (comprenant le numéro de mise à jour) et <debuglevel> est utilisé pour indiquer si le serveur fonctionne en mode débugage.
// Le champ <commentaire> peut contenir n'importe quel commentaire au sujet de la version, ou des détails supplémentaires sur la version.
# define RPL_VERSION(user)	PROMPT(" 351 ", user) + std::string(" ") + SERV_VERSION + std::string(" ") + SERV_NAME + std::string(" :") + SERV_INFO

// En réponse à un message NAMES, une paire consistant de RPL_NAMREPLY et RPL_ENDOFNAMES est renvoyée par le serveur au client.
// S'il n'y a pas de canal résultant de la requête, seul RPL_ENDOFNAMES est retourné.
// L'exception à cela est lorsqu'un message NAMES est envoyé sans paramètre et que tous les canaux et contenus visibles sont renvoyés en une suite de message RPL_NAMEREPLY avec un RPL_ENDOFNAMES indiquant la fin.
# define RPL_NAMREPLY(user, channel)	PROMPT(" 353 ", user) + std::string(" ") + channel->getName() + std::string(" :") + channel->showClientList()
# define RPL_ENDOFNAMES(user, name)		PROMPT(" 366 ", user) + std::string(" ") + name + std::string(" :End of /NAMES list")

// Lorsqu'il répond à un message WHOWAS, un serveur doit utiliser RPL_WHOWASUSER, RPL_WHOISSERVER ou ERR_WASNOSUCHNICK pour chacun des pseudonymes de la liste fournie.
// A la fin de toutes les réponses, il doit y avoir un RPL_ENDOFWHOWAS (même s'il n'y a eu qu'une réponse, et que c'était une erreur).
# define RPL_WHOWASUSER(user, client)		PROMPT(" 314 ", user) + std::string(" ") + client->getNickname() + std::string(" ") + client->getUser() + std::string(" ") + client->getHost() + std::string(" * :") + client->getRealName()
# define RPL_ENDOFWHOWAS(user, nickname)	PROMPT(" 369 ", user) + std::string(" ") + nickname + std::string(" :End of WHOWAS")

// Quand il liste les bannissements actifs pour un canal donné, un serveur doit renvoyer la liste en utilisant les messages RPL_BANLIST et RPL_ENDOFBANLIST.
// Un RPL_BANLIST différent doit être utilisé pour chaque identification de bannissement.
// Après avoir listé les identifications de bannissement (s'il y en a), un RPL_ENDOFBANLIST doit être renvoyé.
# define RPL_ENDOFBANLIST(user, name)		PROMPT(" 368 ", user) + std::string(" ") + name + std::string(" :End of channel ban list")
# define RPL_BANLIST(user, channel, banid)	PROMPT(" 367 ", user) + std::string(" ") + channel->getName() + std::string(" ") + banid

// Un serveur répondant à un message INFO doit envoyer toute sa série d'info en une suite de réponses RPL_INFO, avec un RPL_ENDOFINFO pour indiquer la fin des réponses.
# define RPL_INFO(user, arg)	PROMPT(" 371 ", user) + std::string(" ") + arg
# define RPL_ENDOFINFO(user)	PROMPT(" 374 ", user) + std::string(" :End of /INFO list")

// MOTD (Message Of The Day)
# define RPL_MOTD(user)	PROMPT(" 372 ", user) + std::string(" : WELCOME to Potatoe's Land !")
# define RPL_MOTDSTART(user)	PROMPT(" 375 ", user) + std::string(" :- ") + SERV_NAME + std::string("Message of the day - ")
# define RPL_ENDOFMOTD(user)	PROMPT(" 376 ", user) + std::string(" :End of /MOTD command.")

// RPL_YOUREOPER est renvoyé à un client qui vient d'émettre un message OPER et a obtenu le statut d'opérateur.
# define RPL_YOUREOPER(user)	PROMPT(" 381 ", user) + std::string(" :You are now an IRC operator")

// Lorsqu'il répond à un message TIME, un serveur doit répondre en utilisant le format RPL_TIME.
// La chaîne montrant l'heure ne doit contenir que le jour et l'heure corrects.
// Il n'y a pas d'obligation supplémentaire.
# define RPL_TIME(user, currentTime)	PROMPT(" 391 ", user) + std::string(" ") + SERV_NAME + std::string(":") + currentTime

#endif
