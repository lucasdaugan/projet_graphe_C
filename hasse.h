#ifndef __HASSE_H__
#define __HASSE_H__

#include "adj_list.h"  // tes types adjlist_t, cell_t
#include "tarjan.h"    // t_partition, t_classe

#ifdef __cplusplus
extern "C" {
#endif

    /* ===== Types attendus par le module fourni ===== */

    typedef struct {
        int from; // index de classe source (0..P->count-1)
        int to;   // index de classe destination
    } t_link;

    typedef struct {
        t_link *links;      // tableau dynamique de liens
        int     log_size;   // nombre de liens utilisés
        int     phys_size;  // capacité allouée
    } t_link_array;

    /* ===== API fournie ===== */
    void removeTransitiveLinks(t_link_array *p_link_array);

    /* ===== API que l'on ajoute pour construire les liens et exporter ===== */

    /**
     * @brief Construit le tableau de liens (entre classes) à partir de la partition et du graphe.
     *        Ajoute un lien Ci->Cj s'il existe une arête i->j avec classes différentes.
     *        Évite les doublons et les boucles (Ci==Cj).
     *
     * @param part  Partition (résultat de Tarjan)
     * @param graph Graphe (liste d'adjacence)
     * @return      t_link_array alloué et rempli (à libérer avec hasse_free_links)
     */
    t_link_array hasse_build_links(const t_partition *part, const adjlist_t *graph);

    /**
     * @brief Libère la mémoire du t_link_array.
     */
    void hasse_free_links(t_link_array *A);

    /**
     * @brief (Optionnel) Exporte le diagramme de Hasse (liens entre classes) au format Mermaid.
     * @param part   Partition (pour les noms "C1", "C2", …)
     * @param links  Tableau de liens (éventuellement déjà nettoyé par removeTransitiveLinks)
     * @param filename Chemin du fichier de sortie
     * @return 0 si OK, -1 si erreur ouverture fichier
     */
    int hasse_write_mermaid(const t_partition *part, const t_link_array *links, const char *filename);

#ifdef __cplusplus
}
#endif

#endif // __HASSE_H__