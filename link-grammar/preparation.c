/*************************************************************************/
/* Copyright (c) 2004                                                    */
/* Daniel Sleator, David Temperley, and John Lafferty                    */
/* All rights reserved                                                   */
/*                                                                       */
/* Use of the link grammar parsing system is subject to the terms of the */
/* license set forth in the LICENSE file included with this software.    */
/* This license allows free redistribution and use in source and binary  */
/* forms, with or without modification, subject to certain conditions.   */
/*                                                                       */
/*************************************************************************/

#include "api-structures.h"
#include "build-disjuncts.h"
#include "count.h"
#include "disjunct-utils.h"
#include "externs.h"
#include "preparation.h"
#include "print.h"
#include "prune.h"
#include "resources.h"
#include "string-set.h"
#include "structures.h"
#include "word-utils.h"

static void
set_connector_list_length_limit(Connector *c,
                                Connector_set *conset,
                                int short_len,
                                bool all_short,
                                const char * ZZZ)
{
	for (; c!=NULL; c=c->next)
	{
		if (string_set_cmp (ZZZ, c->string))
		{
			c->length_limit = 1;
		}
		else if (all_short ||
		         (conset != NULL && !match_in_connector_set(conset, c)))
		{
			c->length_limit = short_len;
		}
	}
}

static void
set_connector_length_limits(Sentence sent, Parse_Options opts)
{
	size_t i;
	unsigned int len = opts->short_length;
	bool all_short = opts->all_short;
	Connector_set * ucs = sent->dict->unlimited_connector_set;
	const char * ZZZ = string_set_add("ZZZ", sent->dict->string_set);

	if (0)
	{
		/* Not setting the length_limit saves observable time. However, if we
		 * would like to set the ZZZ connector length_limit to 1 for all
		 * sentences, we cannot do the following.
		 * FIXME(?): Use a flag that the sentence contains an empty word. */
		if (len >= sent->length) return; /* No point to enforce short_length. */
	}

	if (len > UNLIMITED_LEN) len = UNLIMITED_LEN;

	for (i=0; i<sent->length; i++)
	{
		Disjunct *d;
		for (d = sent->word[i].d; d != NULL; d = d->next)
		{
			set_connector_list_length_limit(d->left, ucs, len, all_short, ZZZ);
			set_connector_list_length_limit(d->right, ucs, len, all_short, ZZZ);
		}
	}
}

/**
 * Assumes that the sentence expression lists have been generated.
 */
void prepare_to_parse(Sentence sent, Parse_Options opts)
{
	size_t i;

	build_sentence_disjuncts(sent, opts->disjunct_cost);
	if (verbosity > 2) {
		printf("After expanding expressions into disjuncts:");
		print_disjunct_counts(sent);
	}
	print_time(opts, "Built disjuncts");

	for (i=0; i<sent->length; i++) {
		sent->word[i].d = eliminate_duplicate_disjuncts(sent->word[i].d);

		/* Some long Russian sentences can really blow up, here. */
		if (resources_exhausted(opts->resources))
			return;
	}
	print_time(opts, "Eliminated duplicate disjuncts");

	if (verbosity > 2) {
		printf("\nAfter expression pruning and duplicate elimination:\n");
		print_disjunct_counts(sent);
	}

	set_connector_length_limits(sent, opts);
	pp_and_power_prune(sent, opts);
}

