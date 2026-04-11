#pragma once

enum BeatStatus
{
	eBeatStatus_notRunning = 0,
	eBeatStatus_running = 1,
	eBeatStatus_ambMetaCommentaryMixinPoint = 2,
	eBeatStatus_ambLastMixinPoint = 3,
	eBeatStatus_ambNextDecisionPoint = 4,
	eBeatStatus_completed = 5,
	eBeatStatus_aborted = 6,
	eBeatStatus = 7,
};